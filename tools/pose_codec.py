"""Lossless pose LZSS with literal runs; references the decoded pose cache.

Tokens 0..127: token+1 literal bytes. Tokens 128..255: length=(token&63)+3,
9-bit backward distance=(((token&64)<<2)|next_byte)+1. Matches may overlap.
"""
import collections

def encode(raw):
 out=bytearray();literal=bytearray();pos=0;seen=collections.defaultdict(list)
 def flush():
  if literal:out.append(len(literal)-1);out.extend(literal);literal.clear()
 while pos<len(raw):
  best=0;distance=0
  if pos+3<=len(raw):
   for old in reversed(seen[raw[pos:pos+3]]):
    dist=pos-old
    if dist>512:break
    n=3
    while n<66 and pos+n<len(raw) and raw[old+n]==raw[pos+n]:n+=1
    if n>best:best=n;distance=dist
  n=best if best>=3 else 1
  if n==1:
   literal.append(raw[pos])
   if len(literal)==128:flush()
  else:
   flush();d=distance-1;out.extend([128|((d>>8)<<6)|(n-3),d&255])
  for at in range(pos,pos+n):seen[raw[at:at+3]].append(at)
  pos+=n
 flush();return bytes(out)

def decode(src,n):
 out=bytearray();i=0
 while len(out)<n:
  code=src[i];i+=1
  if code&128:
   dist=((code&64)<<2|src[i])+1;i+=1
   for _ in range((code&63)+3):out.append(out[-dist])
  else:
   count=code+1;out.extend(src[i:i+count]);i+=count
 assert len(out)==n and i==len(src)
 return bytes(out)
