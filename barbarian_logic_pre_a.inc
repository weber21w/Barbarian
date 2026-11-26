/* Extracted from original SDL main.c lines 1163-1572. Do not clean up until trace checks pass. */
      if ( GOTOU == 0 ) { temps = temps + 1 ; }



      ///                    ************* scenes d animation************;





       if(GOTOU==0&&sorcier==1){ GOTOU=23; };


       if ( GOTOU == 0 && bonusA == 1 ) {
          scoreA = scoreA + 10, chronometre = chronometre - 1;
           if ( chronometre < 1 ) {
              chronometre = 0, scoreA = scoreA - 10;
               if ( xB >= 39 ) { sortieA = 1, Aoccupe = 0 ; }
          };
      };

       if ( GOTOU == 0 && bonusB == 1 ) {
          scoreB = scoreB + 10, chronometre = chronometre - 1;
           if ( chronometre < 1 ) {
              chronometre = 0, scoreB = scoreB - 10;
               if ( xA >= 39 ) { sortieB = 1, Boccupe = 0 ; }
          };
      };

      if ( GOTOU == 0 && entree == 1 ) {
          if ( xA >= 13 || xB <= 22 ) { entree = 0 ; }
     };

      if ( GOTOU == 0 && sortieA == 1 ) {

          if ( joueurB == 81 || joueurB == 79 ) {
              if ( xA >= 39 ) {

                  ///******* round solo gagne en inverse;
                   if ( partie == 1 ) {
                       if ( demo == 1 ) {
                                                   Mix_HaltChannel(-1); // stoppe tous les sons
                           if ( transition == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                           if ( GOTOU == 0 && transitionfinie == 1 ) { scoreA = 0, scoreB = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                      };

                       if ( GOTOU == 0 && transition == 1 ) { GOTOU = 26 ; }

                       if ( GOTOU == 0 && transitionfinie == 1 ) { IA = IA + 1 ; }

                       if ( GOTOU == 0 && IA > 7 ) {
                          IA = 7, sorcier = 1;
                          sens = 9;
                          joueurB = 82, joueurA = 1;
                          xA = 36, xB = 8;
                          entree = 0, sortieA = 0;
                          entreesorcier = 1;
                          Boccupe = 1 ;
                          reftempsB = temps;
                          GOTOU = 35;
                      };
                      /// changement de decor;
                       if ( GOTOU == 0 ) {
                           if ( IA == 0 ) { decorS = 2 ; }
                           if ( IA == 1 ) { decorS = 1 ; }
                           if ( IA == 2 ) { decorS = 2 ; }
                           if ( IA == 3 ) { decorS = 1 ; }
                           if ( IA == 4 ) { decorS = 3 ; }
                           if ( IA == 5 ) { decorS = 4 ; }
                           if ( IA == 6 ) { decorS = 3 ; }
                           if ( IA == 7 ) { decorS = 4 ; }

                           if ( demo == 1 ) { decorS = 2 ; }
                      };

                       if ( GOTOU == 0 && transitionfinie == 1 ) { GOTOU = 8 ; }

                       if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 26 ; }
                       if ( GOTOU == 0 ) { GOTOU = 8 ; }
                  };


                 ///******* round VS gagne en inverse;
                  if ( GOTOU == 0 && partie == 2 ) {
                     if ( GOTOU == 0 && transition ==1 ) { GOTOU = 26 ; }
                      if ( GOTOU == 0 && transitionfinie == 1 ) {
                         chronometre = 45; g->chronoPart = 0; chrono = temps;
                                                 Mix_HaltChannel(-1); // stoppe tous les sons
                         if ( decorS == 2 ) { decorS = 1, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 1 ) { decorS = 3, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 3 ) { decorS = 4, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 4 ) { decorS = 2, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 ) { GOTOU = 16 ; }// ex32
                     };
                      if ( GOTOU == 0 ) { transition = 1, reftemps = temps, GOTOU = 26 ; }
                 };
             };
         };



          if ( GOTOU == 0 && tempsfini == 0 ) {
             if ( xA <= 1 && xB >= 39 ) {
                  ///******* round solo gagne;
                   if ( partie == 1 ) {
                       if ( demo == 1 ) {
                            Mix_HaltChannel(-1); // stoppe tous les sons
                           if ( transition == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                           if ( GOTOU == 0 && transitionfinie == 1 ) { scoreA = 0, scoreB = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                      };

                       if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }

                       if ( GOTOU == 0 && transitionfinie == 1 ) { IA = IA + 1 ; }

                       if ( GOTOU == 0 && IA > 7 ) {
                          IA = 7, sorcier = 1;
                          sens = 9;
                          joueurB = 82, joueurA = 1;
                          xA = 36, xB = 8;
                          entree = 0, sortieA = 0;
                          entreesorcier = 1;
                          Boccupe = 1 ;
                          reftempsB = temps;
                          GOTOU = 35;
                      };
                      /// changement de decor;
                       if ( GOTOU == 0 ) {
                           if ( IA == 0 ) { decorS = 2 ; }
                           if ( IA == 1 ) { decorS = 1 ; }
                           if ( IA == 2 ) { decorS = 2 ; }
                           if ( IA == 3 ) { decorS = 1 ; }
                           if ( IA == 4 ) { decorS = 3 ; }
                           if ( IA == 5 ) { decorS = 4 ; }
                           if ( IA == 6 ) { decorS = 3 ; }
                           if ( IA == 7 ) { decorS = 4 ; }

                           if ( demo == 1 ) { decorS = 2 ; }
                      };

                       if ( GOTOU == 0 && transitionfinie == 1 ) { GOTOU = 8 ; }

                       if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                       if ( GOTOU == 0 ) { GOTOU = 8 ; }
                  };


                 ///******* round VS gagne;
                  if ( GOTOU == 0 && partie == 2 ) {

                      if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }
                      if ( GOTOU == 0 && transitionfinie == 1 ) {
                        chronometre = 45; g->chronoPart = 0; chrono = temps;
                                                Mix_HaltChannel(-1); // stoppe tous les sons
                        if ( decorS == 2 ) { decorS = 1, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 1 ) { decorS = 3, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 3 ) { decorS = 4, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 4 ) { decorS = 2, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 ) { GOTOU = 16 ; }// ex32
                     };

                      if ( GOTOU == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }

                 };
             };
        };

       if ( GOTOU == 0 && tempsfini == 1 ) {
              if ( sens == 6 ) {
                  if ( xA < 1 && xB >= 39 ) {
                     chronometre = 45; g->chronoPart = 0; chrono = temps;
                     GOTOU = 16;
                  };
              };
              if ( GOTOU == 0 && sens == 9 ) {
                   if ( xB < 1 && xA >= 39 ) {
                     chronometre = 45; g->chronoPart = 0; chrono = temps;
                     GOTOU = 16;
                   };
              };

         };

          if ( GOTOU == 0 ) { GOTOU = 22 ; }
     };


       if ( GOTOU == 0 && sortieB == 1 ) {
           if ( xB < 1 ) {
               if ( joueurA == 81 || joueurA == 79 ) {

                  ///********** partie solo perdue par mort inversee;
                    if ( partie == 1 ) {
                        if ( demo == 1 ) {
                                                        Mix_HaltChannel(-1); // stoppe tous les sons
                            if ( transition == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                            if ( GOTOU == 0 && transitionfinie == 1 ) { scoreA = 0, scoreB = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                       };

                        if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }
                        if ( GOTOU == 0 && transitionfinie == 1 ) {
                           IA = 0, scoreA = 0, scoreB = 0;
                           decorS = 2, GOTOU = 8;
                       };

                        if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }

                        if ( GOTOU == 0 ) { GOTOU = 8 ; }

                   };

                  ///******* round VS perdu en inverse;
                   if ( GOTOU == 0 && partie == 2 ) {
                       if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }
                       if ( GOTOU == 0 && transitionfinie == 1 ) {
                          chronometre = 45; g->chronoPart = 0; chrono = temps;
                                                   Mix_HaltChannel(-1); // stoppe tous les sons
                           if ( decorS == 2 ) { decorS = 1, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 1 ) { decorS = 3, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 3 ) { decorS = 4, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 4 ) { decorS = 2, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 ) { GOTOU = 16 ; }// ex32
                      };
                       if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }

                   };
              };
          };



           if ( GOTOU == 0 && tempsfini == 0 ) {
               if ( xA >= 39 && xB >= 39 ) {
                  ///********** partie solo perdue;
                    if ( partie == 1 ) {
                        if ( demo == 1 ) {
                                                        Mix_HaltChannel(-1); // stoppe tous les sons
                            if ( transition == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                            if ( GOTOU == 0 && transitionfinie == 1 ) { scoreA = 0, scoreB = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                       };

                        if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }
                        if ( GOTOU == 0 && transitionfinie == 1 ) {
                           IA = 0, scoreA = 0, scoreB = 0;
                           decorS = 2, GOTOU = 8;
                       };

                        if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }
                        if ( GOTOU == 0 ) { GOTOU = 8 ; }
                   };

                  ///******* round VS perdu;
                   if ( GOTOU == 0 && partie == 2 ) {
                       if ( GOTOU == 0 && transition == 1 ) { GOTOU = 31 ; }
                       if ( GOTOU == 0 && transitionfinie == 1 ) {
                          chronometre = 45; g->chronoPart = 0; chrono = temps;
                                                        Mix_HaltChannel(-1); // stoppe tous les sons
                           if ( decorS == 2 ) { decorS = 1, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 1 ) { decorS = 3, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 3 ) { decorS = 4, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 4 ) { decorS = 2, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 ) { GOTOU = 16 ; }// ex32
                      };
                       if ( GOTOU == 0 && sorcier == 0 ) { transition = 1, reftemps = temps, GOTOU = 31 ; }

                  };
              };
          };

           if ( GOTOU == 0 ) { GOTOU = 21 ; }

      };

    if ( GOTOU == 0 && gnome == 1 ) { GOTOU = 12 ; }



     ///                   ************************************;
     ///                   *******GESTION DES DEGATS***********;
     ///                   ************************************;

     //degats ;
      if ( GOTOU == 23 ) { GOTOU = 0 ; }


     //degats sur joueurA;
if ( GOTOU == 0 && ((u8)(spriteA-43)<5 || spriteA==59) ) { GOTOU=26; };

      if(GOTOU==0&&sorcier==1){ BarbarianBossDamage(g,&ctx); };


      if(GOTOU==0){ u8 contact=BarbarianContact(g,0); if(contact){if(!(contact&64))BarbarianClashSound(g);joueurA=contact;GOTOU=26;} }
if ( GOTOU == 0 && Aoccupe == 1 ) { GOTOU = 26 ; } // important pour les animations
      if ( GOTOU == 0 ) { sangA = 0 ; }
