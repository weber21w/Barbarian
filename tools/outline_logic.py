"""Outline identical straight-line AVR logic blocks, preserving registers/flags.

Only the three game logic functions participate. Hardware/ISR/compositor code,
branches, stack operations, skip-next instructions and their skipped instruction
are excluded. CALL/RET adds two transient stack bytes and leaves SREG unchanged.
The exact instruction sequence at every replacement is checked before emission.
"""
import re, collections

def interruptible_contact_return(source):
    """Replace verified frame-zero Contact shared entry/return sequences.

    libgcc's shared epilogue masks IRQs while writing SP. Explicit POP/RET
    preserves the saved registers and stack without that hardware sequence.
    """
    match=re.search(r'(?ms)^BarbarianContact:\n(.*?)^(\s*\.size\s+BarbarianContact,[^\n]*\n)',source)
    if not match:return source,dict(present=False,rewritten=False)
    body=match[1]
    if not re.search(r'/\* frame size = 0 \*/',body):raise ValueError('Contact return requires a zero-byte local frame')
    prologue=body.split('/* prologue:',1)[0]
    saved=re.findall(r'(?m)^\s*push\s+(r\d+)\s*$',prologue)
    shared=re.fullmatch(r'\s*ldi r26,0\n\s*ldi r27,0\n\s*ldi r30,lo8\(gs\(1f\)\)\n\s*ldi r31,hi8\(gs\(1f\)\)\n\s*jmp __prologue_saves__\+\(\(18 - (\d+)\) \* 2\)\n1:\s*',prologue)
    if shared:
        count=int(shared[1])
        if not 2<=count<=18:raise ValueError('Unexpected Contact saved-register count')
        saved=['r'+str(n) for n in range(20-count,18)]+['r28','r29']
        body=''.join('\tpush '+reg+'\n' for reg in saved)+body[len(prologue):]
    rewritten=False
    if '__epilogue_restores__' in body:
        pattern=r'(?m)^\s*in\s+r28,\s*__SP_L__\s*\n\s*in\s+r29,\s*__SP_H__\s*\n\s*ldi\s+r30,[^\n]*\n\s*jmp\s+__epilogue_restores__[^\n]*\n'
        exits=list(re.finditer(pattern,body));n=len(saved)
        expected=f'in r28,__SP_L__ in r29,__SP_H__ ldi r30,lo8({n}) jmp __epilogue_restores__ + ((18 - {n}) * 2)'
        if len(exits)!=1 or re.sub(r'\s+','',exits[0][0])!=re.sub(r'\s+','',expected):raise ValueError('Unexpected Contact shared epilogue')
        replacement=''.join('\tpop '+reg+'\n' for reg in reversed(saved))+'\tret\n'
        body=body[:exits[0].start()]+replacement+body[exits[0].end():];rewritten=True
    if re.search(r'(?m)^\s*(?:cli|sei|in|out|call|rcall)\b',body) or '__epilogue_restores__' in body or '__prologue_saves__' in body:raise ValueError('Contact path must remain interruptible and local')
    return source[:match.start(1)]+body+source[match.end(1):],dict(present=True,rewritten=rewritten,saved_registers=saved)


def outline(source):
    lines = source.splitlines()
    allowed = {'mov', 'movw', 'ldi', 'clr', 'ser', 'add', 'adc', 'sub', 'sbc', 'subi', 'sbci', 'and', 'andi', 'or', 'ori', 'eor', 'com', 'neg', 'inc', 'dec', 'swap', 'lsl', 'lsr', 'rol', 'ror', 'asr', 'mul', 'muls', 'mulsu', 'cp', 'cpc', 'cpi', 'tst', 'ld', 'ldd', 'lds', 'st', 'std', 'sts', 'adiw', 'sbiw'}
    blocks = []
    block = []
    enabled = False
    skip_next = False
    for (n, line) in enumerate(lines):
        if line.startswith('\t.section\t'):
            enabled = bool(re.match('\\s*\\.section\\s+\\.text\\.BarbarianLogic(?:PreA|PostAPreB|PostB),', line))
        m = re.match('^\\s*([a-z]+)\\s*(.*)$', line)
        op = m[1] if m else ''
        if enabled and m and (op in ('sbrc', 'sbrs', 'sbic', 'sbis', 'cpse') or re.search('\\.\\s*\\+\\s*[1-9]\\d*', m[2])):
            following = re.match('^\\s*([a-z]+)\\s*(.*)$', lines[n + 1])
            assert following, ('Unmodeled directive after skip', n, line)
            relative = re.search('\\.\\s*\\+\\s*(\\d+)', m[2])
            if relative:
                width = 4 if following[1] in ('jmp', 'call', 'lds', 'sts') else 2
                assert int(relative[1]) == width, ('Relative branch spans multiple instructions', n, line)
        if enabled and m and (op in allowed) and (not skip_next):
            block.append((n, op + ' ' + m[2].strip()))
        elif block:
            blocks.append(block)
            block = []
        skip_next = op in ('sbrc', 'sbrs', 'sbic', 'sbis', 'cpse') or bool(m and re.search('\\.\\s*\\+\\s*\\d', m[2]))
    if block:
        blocks.append(block)
    plans = []
    saved = 0
    while True:
        candidates = collections.defaultdict(list)
        for (bi, b) in enumerate(blocks):
            for n in range(2, min(30, len(b)) + 1):
                for at in range(len(b) - n + 1):
                    candidates[tuple((x[1] for x in b[at:at + n]))].append((bi, at))
        best = None
        for (body, places) in candidates.items():
            size = sum((4 if i.startswith(('lds ', 'sts ')) else 2 for i in body))
            chosen = []
            last = {}
            for (bi, at) in places:
                if at >= last.get(bi, 0):
                    chosen.append((bi, at))
                    last[bi] = at + len(body)
            saving = len(chosen) * (size - 4) - (size + 2)
            if best is None or saving > best[0]:
                best = (saving, body, chosen, size)
        if best is None or best[0] <= 0:
            break
        (saving, body, places, size) = best
        saved += saving
        plan = {'instructions': list(body), 'bytes': size, 'occurrences': len(places), 'saving': saving, 'lines': []}
        byblock = collections.defaultdict(list)
        for (bi, at) in places:
            byblock[bi].append(at)
            plan['lines'].append([n for (n, _) in blocks[bi][at:at + len(body)]])
        new = []
        for (bi, b) in enumerate(blocks):
            begin = 0
            for at in byblock[bi]:
                if at > begin:
                    new.append(b[begin:at])
                begin = at + len(body)
            if begin < len(b):
                new.append(b[begin:])
        blocks = new
        plans.append(plan)
        if len(plans) >= 200:
            break
    replacements = {}
    removed = set()
    helpers = []
    for (index, plan) in enumerate(plans):
        name = 'barbLogicOutline_' + str(index)
        for positions in plan['lines']:
            assert positions == list(range(positions[0], positions[0] + len(positions)))
            original = []
            for at in positions:
                m = re.match('^\\s*([a-z]+)\\s*(.*)$', lines[at])
                assert m
                original.append(m[1] + ' ' + m[2].strip())
            assert original == plan['instructions']
            assert not set(positions) & removed
            replacements[positions[0]] = '\tcall ' + name
            removed.update(positions)
        helpers += ['.type ' + name + ', @function', name + ':', *('\t' + i for i in plan['instructions']), '\tret', '.size ' + name + ', .-' + name]
    changed = []
    for (at, line) in enumerate(lines):
        if at in replacements:
            changed.append(replacements[at])
        elif at not in removed:
            changed.append(line)
    # Put the shared instruction pool between the two largest logic passes.
    # The linker can then relax nearby CALLs to RCALLs without changing the
    # outlined instructions, their register/flag ABI, or transient stack depth.
    pool=['.pushsection .text.barbLogicOutlines,"ax",@progbits', *helpers, '.popsection']
    slots=[i for i,line in enumerate(changed) if re.match(r'\s*\.section\s+\.text\.BarbarianLogicPostB,',line)]
    if len(slots)!=1:raise ValueError('Expected one PostB section for outline placement')
    changed[slots[0]:slots[0]]=pool
    assembly,contact_return=interruptible_contact_return('\n'.join(changed) + '\n')
    return (assembly, dict(pool_placement='before PostB',estimated_saved=saved, helpers=len(plans), calls=len(replacements), plans=plans,contact_return=contact_return))
