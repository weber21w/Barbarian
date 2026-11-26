/* Extracted from original SDL main.c lines 3666-6115. Do not clean up until trace checks pass. */
     ///                       **********************************;
     ///                       ******* animations  auto *********;
     ///                       **********************************;

        if ( GOTOU == 0 && entree == 1 ) {
                levier2 = 4, GOTOU = 20;
        };


      if ( GOTOU == 0 && sortieB == 1 ) {
          if ( GOTOU == 0 && sens == 9 ) { levier2 = 4, GOTOU = 20 ; }

          if ( GOTOU == 0 ) { sens = 6, levier2 = 6, GOTOU = 20 ; }
     };


      ///                       *****************************************;
      ///                       ******* Intelligence Artificielle *******;
      ///                       *****************************************;


       if ( GOTOU == 0 && partie == 1 ) {

          // ***************************IA de 1,2,3,6;
           if ( IA == 0 || IA == 1 || IA == 2 || IA == 3 || IA == 6 ) {
               if ( sens == 6 ) {
                  distance = xB - xA;

                   if ( temps <= 140 ) { /* bug shining */
                           if ( joueurA == 58 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                  };

                   if ( GOTOU == 0 && distance >= 15 ) { //  quand trop loin;
                      joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                  };

                   if ( GOTOU == 0 && IA == 6 ) {
                       if ( distance < 15 ) {
                           if ( joueurA == 58 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && IA == 3 ) {
                       if ( distance < 15 ) {
                           if ( infoBdegatT > 2 ) {
                               if ( joueurA == 58 ) { joueurB = 27, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && joueurA == 58 ) { joueurB = 43, reftempsB = temps, GOTOU = 24 ; }
                      };
                  };


                   if ( GOTOU == 0 && distance == 12 && joueurA == 0 ) { joueurB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                   if ( GOTOU == 0 && distance > 9 && distance < 15 ) { // pour se rapprocher;
                       if ( levier1 == 4 ) { joueurB = 0, GOTOU = 24 ; }
                       if ( GOTOU == 0 ) { levier2 = 4, GOTOU = 20 ; }
                  };

                   if ( GOTOU == 0 && distance == 9 ) {
                       if ( attenteA > 50 ) { levier2 = 4, GOTOU = 20 ; }
                       if ( GOTOU == 0 && joueurA == 36 ) { joueurB = 48, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && Aoccupe == 1 ) { levier2 = 4, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance < 9 && distance > 6 ) { // distance de combat 1;
                      // pour autoriser les croisements;
                       if ( demo == 0 && joueurA == 32 ) { joueurB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                      // pour se rapprocher;
                       if ( GOTOU == 0 && joueurA == 36 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && levier1 == 4 ) { joueurB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                      //pour eviter les degats repetitifs;

                       if ( GOTOU == 0 && IA > 1 ) {
                           if ( infoBdegatG > 4 ) {
                               if ( joueurA == 27 || joueurA == 50 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 27 || joueurA == 32 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ) {
                               if ( joueurA == 46 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ) {
                               if ( joueurA == 60 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                      };


                      //pour alterner les attaques;
                       BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence0);
           };

                   if ( GOTOU == 0 && distance <= 6 ) {

                      // pour autoriser les croisements;
                       if ( demo == 0 && joueurA == 24 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && IA == 3 ) {
                           if ( joueurA == 48 ) { joueurB = 43, reftempsB = temps, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA == 2 ) {
                           if ( joueurA == 50 ) { joueurB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA > 1 ) {
                           if ( infoBdegatG > 4 ) {
                               if ( joueurA == 27 || joueurA == 50 ) { joueurB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 && IA > 2 ) {
                                   if ( joueurA == 52 ) { joueurB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 54 ) { joueurB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ) {
                                  if ( joueurA == 27 || joueurA == 50 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                      };

                       BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence1);
           };
              };


               if ( GOTOU == 0 && sens == 9 ) {

                  distance = xA - xB;

                   if ( GOTOU == 0 && distance >= 15 ) { //  quand trop loin;
                      joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                  };

                   if ( GOTOU == 0 && IA == 6 ) {
                       if ( distance < 15 ) {
                           if ( joueurA == 59 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && IA == 3 ) {
                       if ( distance < 15 ) {
                           if ( infoBdegatT > 2 ) {
                               if ( joueurA == 59 ) { joueurB = 30, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && joueurA == 59 ) { joueurB = 45, reftempsB = temps, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && distance == 12 && joueurA == 1 ) { joueurB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                   if ( GOTOU == 0 && distance > 9 && distance < 15 ) { // pour se rapprocher;
                       if ( levier1 == 6 ) { joueurB = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 ) { levier2 = 6, GOTOU = 20 ; }
                  };

                   if ( GOTOU == 0 && distance == 9 ) {
                       if ( attenteA > 50 ) { levier2 = 6, GOTOU = 20 ; }
                       if ( GOTOU == 0 && joueurA == 35 ) { joueurB = 49, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && Aoccupe == 1 ) { levier2 = 6, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance < 9 && distance > 6 ) { // distance de combat 1;
                      // pour autoriser les croisements;
                       if ( demo == 0 && joueurA == 34 ) { joueurB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                      // pour se rapprocher;
                       if ( GOTOU == 0 && joueurA == 35 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && levier1 == 6 ) { joueurB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                      //pour eviter les degats repetitifs;

                       if ( GOTOU == 0 && IA > 1 ) {
                           if ( infoBdegatG > 4 ) {
                               if ( joueurA ==9102 || joueurA == 51 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 30 || joueurA == 34 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ) {
                               if ( joueurA == 47 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ) {
                               if ( joueurA == 61 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                      };


                      //pour alterner les attaques;
                       BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence2);
           };

                   if ( GOTOU == 0 && distance <= 6 ) {

                      // pour autoriser les croisements;
                       if ( demo == 0 && joueurA == 25 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && IA == 3 ) {
                           if ( joueurA == 49 ) { joueurB = 45, reftempsB = temps, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA == 2 ) {
                           if ( joueurA == 51 ) { joueurB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };


                       if ( GOTOU == 0 && IA > 1 ) {
                           if ( infoBdegatG > 4 ) {
                               if ( joueurA == 30 || joueurA == 51 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 && IA > 2 ) {
                                   if ( joueurA == 53 ) { joueurB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 55 ) { joueurB = 55, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0  ) {
                                  if ( joueurA == 30 || joueurA == 55 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                      };

                       BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence3);
           };
              };

          };


          // ************************* IA de 4,5,6,7;

           if ( GOTOU == 0 ) {
               if ( IA == 4 || IA == 5 || IA == 6 || IA == 7 ) {

                   if ( sens == 6 ) {
                      distance = xB - xA;

                       if ( GOTOU == 0 && distance >= 15 ) { //  quand trop loin;
                          joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                      };


                       if ( GOTOU == 0 && distance == 12 && joueurA == 0 ) { joueurB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && distance > 9 && distance < 15 ) { // pour se rapprocher;
                           if ( levier1 == 4 ) { joueurB = 0, GOTOU = 24 ; }
                           if ( GOTOU == 0 ) { levier2 = 4, GOTOU = 20 ; }
                      };

                       if ( GOTOU == 0 && distance == 9 ) {
                           if ( attenteA > 50 ) { joueurB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && demo == 1 ) {
                               if ( attenteA > 50 ) { joueurB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && joueurA == 36 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // if ( GOTOU == 0 && IA < 7 ) {
                          //     if ( GOTOU == 0 && Aoccupe == 1 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // ; }
                           if ( GOTOU == 0 && IA == 7 ) {
                               if ( Aoccupe == 1 ) { levier2 = 4, GOTOU = 20 ; }
                          };
                      };



                       if ( GOTOU == 0 && distance < 9 && distance > 6 ) { // distance de combat 1;
                          // pour autoriser les croisements;
                           if ( demo == 0 && joueurA == 32 ) { joueurB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                          // pour se rapprocher;
                           if ( GOTOU == 0 && joueurA == 36 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && levier1 == 4 ) { joueurB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                          //plus l IA est forte, plus il y des des coups imposes avant infocoupB ou infodegat;

                           if ( GOTOU == 0 && IA == 5 ) {
                               if ( joueurA == 60 ) { joueurB = 39, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && distance < 15 && demo == 1 ) {
                              joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                          };

                          //pour eviter les degats repetitifs;
                           if ( GOTOU == 0 && infoBdegatG > 4 ) {
                               if ( joueurA == 27 || joueurA == 50 || joueurA == 52 ) { joueurB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 27 || joueurA == 50 || joueurA == 52 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ) {
                               if ( joueurA == 46 ) { joueurB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ) {
                               if ( IA < 7 ) {
                                   if ( joueurA == 60 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };


                          //pour alterner les attaques;

                           BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence4);
           };

                       if ( GOTOU == 0 && distance <= 6 ) {

                          // pour autoriser les croisements;
                           if ( demo == 0 && joueurA == 24 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                           if ( GOTOU == 0 && IA > 4 ) {
                               if ( joueurA == 48 ) { joueurB = 43, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && IA > 4 && IA < 7 ) {
                               if ( joueurA == 50 ) { joueurB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatG > 4 ) {
                               if ( joueurA == 27 || joueurA == 50 || joueurA == 52 ) { joueurB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 54 ) { joueurB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ) {
                                   if ( joueurA == 27 || joueurA == 50 || joueurA == 52 ) { joueurB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };

                           BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence5);
           };

                  };


                   if ( GOTOU == 0 && sens == 9 ) {

                      distance = xA - xB;

                       if ( GOTOU == 0 && distance >= 15 ) { //  quand trop loin;
                          joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                      };

                      // if ( GOTOU == 0 && distance < 15 ) {
                      // if ( GOTOU == 0 && IA == 7 ) {
                      // if ( GOTOU == 0 && joueurA == 59 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      // ; }
                      // ; }

                       if ( GOTOU == 0 && distance == 12 && joueurA == 1 ) { joueurB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && distance > 9 && distance < 15 ) { // pour se rapprocher;
                           if ( GOTOU == 0 && levier1 == 6 ) { joueurB = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 ) { levier2 = 6, GOTOU = 20 ; }
                      };

                       if ( GOTOU == 0 && distance == 9 ) {
                           if ( GOTOU == 0 && attenteA > 50 ) { joueurB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && demo == 1 ) {
                               if ( attenteA > 50 ) { joueurB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && joueurA == 35 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // if ( GOTOU == 0 && IA < 7 ) {
                          // if ( GOTOU == 0 && Aoccupe == 1 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // ; }
                           if ( GOTOU == 0 && IA == 7 ) {
                               if ( Aoccupe == 1 ) { levier2 = 6, GOTOU = 20 ; }
                          };
                      };

                       if ( GOTOU == 0 && distance < 9 && distance > 6 ) { // distance de combat 1;
                          // pour autoriser les croisements;
                           if ( GOTOU == 0 && demo == 0 && joueurA == 32 ) { joueurB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                          // pour se rapprocher;
                           if ( GOTOU == 0 && joueurA == 35 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && levier1 == 6 ) { joueurB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                          //plus l IA est forte, plus il y des des coups imposes avant infocoupB ou infodegat;

                           if ( GOTOU == 0 && IA == 5 ) {
                               if ( GOTOU == 0 && joueurA == 61 ) { joueurB = 41, reftempsB = temps, GOTOU = 24 ; }
                          };

                          //pour eviter les degats repetitifs;
                           if ( GOTOU == 0 && infoBdegatG > 4 ) {
                               if ( joueurA == 30 || joueurA == 51 || joueurA == 53 ) { joueurB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( joueurA == 30 || joueurA == 51 || joueurA == 53 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ) {
                               if ( GOTOU == 0 && joueurA == 47 ) { joueurB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ) {
                               if ( GOTOU == 0 && IA < 7 ) {
                                   if ( GOTOU == 0 && joueurA == 61 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };


                          //pour alterner les attaques;

                           BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence6);
           };

                       if ( GOTOU == 0 && distance <= 6 ) {

                          // pour autoriser les croisements;
                           if ( GOTOU == 0 && demo == 0 && joueurA == 25 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                           if ( GOTOU == 0 && IA > 4 ) {
                               if ( GOTOU == 0 && joueurA == 49 ) { joueurB = 45, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && IA > 4 && IA < 7 ) {
                               if ( GOTOU == 0 && joueurA == 51 ) { joueurB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatG > 4 ) {
                               if ( GOTOU == 0  ) {
                                  if ( joueurA == 30 || joueurA == 51 || joueurA == 53 ) { joueurB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ) {
                               if ( GOTOU == 0 && joueurA == 55 ) { joueurB = 55, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ) {
                                   if ( joueurA == 30 || joueurA == 51 || joueurA == 53 ) { joueurB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };

                           BarbarianAiSequence(g, &GOTOU, temps, &reftempsB, barbAiSequence7);
           };

                  };

              };
          };

      };




     //                       redirection suivant les touches;

      if ( GOTOU == 0 && levier2 == 1 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 3 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 7 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 9 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 8 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 2 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 4 ) { GOTOU = 20 ; }
      if ( GOTOU == 0 && levier2 == 6 ) { GOTOU = 20 ; }


     //                       actions si aucune touche n//a ete touchee;

      if ( GOTOU == 0 && (joueurB == 43 || joueurB == 45) ) { joueurB=sens+22; protegeDB=2; Boccupe=1; reftempsB=temps; GOTOU=24; };
if ( GOTOU == 0 ) {
         spriteavanceB = 0;
         spritereculeB = 0;

         protegeDB = 0, protegeHB = 0;

         attenteB = attenteB + 1;
         levier2 = 5;
     };

     // pour se relever;
      if ( GOTOU == 0 ) { assisB = 0 ; }
      if ( GOTOU == 0 && joueurB == 27 ) { joueurB = 28, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
      if ( GOTOU == 0 && joueurB == 30 ) { joueurB = 31, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }


     //attente des 5 secondes;
       if ( GOTOU == 0 && sens == 6 ) {
          if ( attenteB > 300 ) { joueurB = 2, Boccupe = 1, reftempsB = temps, GOTOU = 24; }
     };
      if ( GOTOU == 0 && sens == 9 ) {
          if ( attenteB > 300 ) { joueurB = 3, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
     };

      if ( GOTOU == 0 && sens == 6 ) { joueurB = 0 ; }
      if ( GOTOU == 0 && sens == 9 ) { joueurB = 1 ; }

     if ( GOTOU == 0 ) { GOTOU = 24 ; }


     ///                       *****************************************;
     ///                       *************actions joueur 2************;
     ///                       *****************************************;
     //actionB ;
      if ( GOTOU == 20 ) { GOTOU = 0 ; }

      if ( GOTOU == 0 ) { attenteB = 1 ; }


     //                       droite,gauche, decapite, devant (normal);


      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier2 == 4 ) {
             protegeDB = 0;
              if ( GOTOU == 0 && spriteavanceB == 1 ) { joueurB = 5, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 2 ) { joueurB = 6, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 3 ) { joueurB = 7, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 4 ) { joueurB = 8, GOTOU = 24 ; }
              if ( GOTOU == 0 && attaqueB == 1 && entree == 0 ) { joueurB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 4, reftempsB = temps, GOTOU = 24 ; }
         };

          if ( GOTOU == 0 && levier2 == 6 ) {
             protegeHB = 0;
              if ( GOTOU == 0 && spritereculeB == 1 ) { joueurB = 15, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 2 ) { joueurB = 16, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 3 ) { joueurB = 17, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 4 ) { joueurB = 18, GOTOU = 24 ; }
              if ( GOTOU == 0 && attaqueB == 1 && sortieB == 0 ) { joueurB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 14, reftempsB = temps, GOTOU = 24 ; }
         };
     };


      //                       droite, gauche, decapite, devant (inverse);

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 6 ) {
              protegeDB = 0;
               if ( GOTOU == 0 && spriteavanceB == 1 ) { joueurB = 10, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 2 ) { joueurB = 11, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 3 ) { joueurB = 12, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 4 ) { joueurB = 13, GOTOU = 24 ; }
               if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 9, reftempsB = temps, GOTOU = 24 ; }
          };

           if ( GOTOU == 0 && levier2 == 4 ) {
              protegeHB = 0;
               if ( GOTOU == 0 && spritereculeB == 1 ) { joueurB = 20, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 2 ) { joueurB = 21, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 3 ) { joueurB = 22, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 4 ) { joueurB = 23, GOTOU = 24 ; }
               if ( GOTOU == 0 && attaqueB == 1 && sortieB == 0 ) { joueurB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 19, reftempsB = temps, GOTOU = 24 ; }
          };
      };


     //                           saute, attaque cou;

      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier2 == 2 ) {
             protegeDB = 0, protegeHB = 0;
             joueurB = 24, Boccupe = 1, reftempsB = temps, GOTOU = 24;
         };
     };

      if ( GOTOU == 0 && sens == 9 ) {
          if ( levier2 == 2 ) {
             protegeDB = 0, protegeHB = 0;
             joueurB = 25, Boccupe = 1, reftempsB = temps, GOTOU = 24;
         };
     };


      //                               assis, attaque genou;
       if ( GOTOU == 0 && sens == 6 ) {
           if ( levier2 == 8 ) {
               if ( attaqueB == 1 ) { joueurB = 50, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && assisB == 1 ) { joueurB = 27, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 26, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };
       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 8 ) {
               if ( attaqueB == 1 ) { joueurB = 51, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && assisB == 1 ) { joueurB = 30, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 29, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                               roulade AV, coup de pied;
       if ( GOTOU == 0 && sens == 6 ) {
           if ( levier2 == 7 ) {
               if ( attaqueB == 1 ) { joueurB = 54, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 32, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 9 ) {
               if ( attaqueB == 1 ) { joueurB = 55, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 34, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                               roulade AR, coup sur front;
       if ( GOTOU == 0 && sens == 6 ) {
           if ( levier2 == 9 ) {
               if ( attaqueB == 1 ) { joueurB = 60, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 36, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 7 ) {
               if ( attaqueB == 1 ) { joueurB = 61, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ) { joueurB = 37, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

      //                                   protection Haute, araignee;
       if ( GOTOU == 0 && sens == 6 ) {
           if ( levier2 == 3 ) {
               if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 52, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeHB == 1 ) { joueurB = 39, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 38, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 1 ) {
               if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 53, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeHB == 1 ) { joueurB = 41, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 40, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                                   protection devant, coup de tete;
       if ( GOTOU == 0 && sens == 6 ) {
           if ( levier2 == 1 ) {
               if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 56, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && protegeDB == 1 ) { joueurB = 43, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 42, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier2 == 3 ) {
               if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 57, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeDB == 1 ) { joueurB = 45, GOTOU = 24 ; }
               if ( GOTOU == 0 ) { joueurB = 44, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


     ///                       ***********************************;
     ///                       *********gestion joueur 2**********;
     ///                       ***********************************;


     //gestionB ;
      barb_management_B:;
if ( GOTOU == 24 ) { GOTOU = 0 ; }

      if ( GOTOU == 0 && joueurB == 0 ) {
    spriteB = 0; decapiteB = 1; sangB = 0; xAttB = xB + 4; yAttB = 14; BarbarianBodyY(g, 1U); BarbarianBodyX(g, 1U, 0U);
    if ( GOTOU == 0 && partie == 1 ) {
        if ( GOTOU == 0 && temps > reftempsB + 20 ) {
            Boccupe = 0;
        };
    };
};

      if ( GOTOU == 0 && joueurB == 1 ) {
    spriteB = 0; decapiteB = 1; sangB = 0; xAttB = xB; yAttB = 14; BarbarianBodyY(g, 1U); BarbarianBodyX(g, 1U, 15U);
    if ( GOTOU == 0 && partie == 1 ) {
        if ( GOTOU == 0 && temps > reftempsB + 20 ) {
            Boccupe = 0;
        };
    };
};

      if ( GOTOU == 0 && joueurB == 2 ) {
    BarbarianBodyX(g, 1U, 0U);
    if ( GOTOU == 0 && temps > reftempsB + 50 ) {
        Boccupe = 0; attenteB = 1; joueurB = 0; GOTOU = 2;
    };
    BarbarianTimeline(g, &ctx, barbTimeline53, 1U);

};

      if ( GOTOU == 0 && joueurB == 3 ) {
    BarbarianBodyX(g, 1U, 15U);
    if ( GOTOU == 0 && temps > reftempsB + 50 ) {
        Boccupe = 0; attenteB = 1; joueurB = 1; GOTOU = 2;
    };
    BarbarianTimeline(g, &ctx, barbTimeline54, 1U);

};

     //                                   avance
      if ( GOTOU == 0 ) { BarbarianHandleMoveB(g, &GOTOU, temps, &reftempsB); }

      if ( GOTOU == 0 ) { BarbarianHandleJump(g, &GOTOU, temps, &reftempsB, 1U); };

       ;

       if ( GOTOU == 0 ) { BarbarianHandleCrouch(g, &ctx, 1U); };

       ;

       ;


       ;

       ;

       ;


       if ( GOTOU == 0 && joueurB == 32 ) {
    BarbarianBodyX(g, 1U, 7U); yGB  =  20; yAttB  =  yGB; xAttB  =  xB + 4;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB  =  54; Boccupe  =  1; reftempsB  =  temps; GOTOU  =  24;
    };

BarbarianTimeline(g, &ctx, barbTimeline55, 1U);

u8 outgoing = BarbarianContact(g,2); if(outgoing){joueurA = outgoing;Aoccupe = 1;}
};

      ///finderouladeB ;
       if ( GOTOU == 6 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurB == 33 ) {
    if ( GOTOU == 0 && xB > xA ) {
        joueurB = 0; spriteB = 0; xAttB = xB + 4; yAttB = 17; BarbarianBodyX(g, 1U, 0U); BarbarianBodyY(g, 1U); GOTOU = 2;
    };
    if ( GOTOU == 0 && xB < xA + 1 ) {
        joueurA = 62; yAttA = 14; Aoccupe = 1; reftemps = temps; joueurB = 62; yAttB = 14; Boccupe = 1; reftempsB = temps; GOTOU = 31;
    };
};

       if ( GOTOU == 0 && joueurB == 34 ) {
    BarbarianBodyX(g, 1U, 8U); yGB  =  20; yAttB  =  yGB; xAttB  =  xB;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB  =  55; Boccupe  =  1; reftempsB  =  temps; GOTOU  =  24;
    };

BarbarianTimeline(g, &ctx, barbTimeline56, 1U);

u8 outgoing = BarbarianContact(g,2); if(outgoing){joueurA = outgoing;Aoccupe = 1;}
};

      //finderouladeBR ;
       if ( GOTOU == 7 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurB == 35 ) {
    if ( GOTOU == 0 && xB < xA ) {
        joueurB = 1; spriteB = 0; xAttB = xB; yAttB = 17; BarbarianBodyX(g, 1U, 15U); BarbarianBodyY(g, 1U); GOTOU = 2;
    };
    if ( GOTOU == 0 && xB > xA - 1 ) {
        joueurA = 63; Aoccupe = 1; reftemps = temps; joueurB = 63; Boccupe = 1; reftempsB = temps; GOTOU = 31;
    };
};

       if ( GOTOU == 0 && joueurB == 36 ) {
    BarbarianBodyX(g, 1U, 7U); yGB  =  20; yAttB  =  yGB; xAttB  =  xB + 4;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB  =  60; Boccupe  =  1; reftempsB  =  temps; GOTOU  =  24;
    };

BarbarianTimeline(g, &ctx, barbTimeline57, 1U);

};

       if ( GOTOU == 0 && joueurB == 37 ) {
    BarbarianBodyX(g, 1U, 8U); yGB  =  20; yAttB  =  yGB; xAttB  =  xB;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB  =  61; Boccupe  =  1; reftempsB  =  temps; GOTOU  =  24;
    };

BarbarianTimeline(g, &ctx, barbTimeline58, 1U);

};

       if ( GOTOU == 0 && joueurB == 38 ) {
BarbarianBodyX(g, 1U, 0U);
xAttB = xB + 4; yGB = 20;
if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 52; Boccupe = 1; reftempsB = temps; GOTOU = 24; };
BarbarianTimeline(g, &ctx, barbTimeline59, 1U);

};

       if ( GOTOU == 0 && joueurB == 39 ) {
    BarbarianBodyX(g, 1U, 0U); xAttB = xB + 4; yGB = 20; spriteB = 17;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB = 52; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
};

       if ( GOTOU == 0 && joueurB == 40 ) {
BarbarianBodyX(g, 1U, 15U);
xAttB = xB; yGB = 20;
if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 53; Boccupe = 1; reftempsB = temps; GOTOU = 24; };
BarbarianTimeline(g, &ctx, barbTimeline60, 1U);

};

       if ( GOTOU == 0 && joueurB == 41 ) {
    BarbarianBodyX(g, 1U, 15U); xAttB = xB; yGB = 20; spriteB = 17;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB = 53; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
};

       if ( GOTOU == 0 && joueurB == 42 ) {
BarbarianBodyX(g, 1U, 0U);
xAttB = xB + 4; yGB = 20;
if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 56; Boccupe = 1; reftempsB = temps; GOTOU = 24; };
decapiteB = 1;
BarbarianTimeline(g, &ctx, barbTimeline61, 1U);

};

       if ( GOTOU == 0 && joueurB == 43 ) {
    xAttB = xB + 4; yGB = 20; BarbarianBodyX(g, 1U, 0U); decapiteB = 0; spriteB = 18;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB = 56; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
};

       if ( GOTOU == 0 && joueurB == 44 ) {
BarbarianBodyX(g, 1U, 15U);
xAttB = xB; yGB = 20;
if ( GOTOU == 0 && attaqueB == 1 ) { joueurB = 57; Boccupe = 1; reftempsB = temps; GOTOU = 24; };
decapiteB = 1;
BarbarianTimeline(g, &ctx, barbTimeline62, 1U);

};

       if ( GOTOU == 0 && joueurB == 45 ) {
    xAttB = xB; yGB = 20; BarbarianBodyX(g, 1U, 15U); decapiteB = 0; spriteB = 18;
    if ( GOTOU == 0 && attaqueB == 1 ) {
        joueurB = 57; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
};


       if ( GOTOU == 0 && joueurB == 46 ) {
    BarbarianBodyX(g, 1U, 0U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline63, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 20;
        if ( GOTOU == 0 && joueurA == 46 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xTB = xB + 4; xAttB = xB - 3; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline64, 1U);

};

       if ( GOTOU == 0 && joueurB == 47 ) {
    BarbarianBodyX(g, 1U, 15U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline65, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 20;
        if ( GOTOU == 0 && joueurA == 47 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xTB = xB; xAttB = xB + 7; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline66, 1U);

};

       if ( GOTOU == 0 && joueurB == 48 ) {
    BarbarianBodyX(g, 1U, 0U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline67, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 23;
        if ( GOTOU == 0 && joueurA == 48 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xMB = xB + 4; xAttB = xB - 2; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline68, 1U);

    if ( GOTOU == 0 ) { spriteB = 21; };
};

       if ( GOTOU == 0 && joueurB == 49 ) {
    BarbarianBodyX(g, 1U, 15U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline69, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 23;
        if ( GOTOU == 0 && joueurA == 49 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xMB = xB; xAttB = xB + 6; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline70, 1U);

    if ( GOTOU == 0 ) { spriteB = 21; };
};

       if ( GOTOU == 0 && joueurB == 50 ) {
    BarbarianBodyX(g, 1U, 7U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline71, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 25;
        if ( GOTOU == 0 && joueurA == 50 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xGB = xB + 4; xAttB = xB - 3; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline72, 1U);

    if ( GOTOU == 0 ) { spriteB = 24; };
};

       if ( GOTOU == 0 && joueurB == 51 ) {
    BarbarianBodyX(g, 1U, 8U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline73, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 25;
        if ( GOTOU == 0 && joueurA == 51 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xGB = xB; xAttB = xB + 7; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline74, 1U);

    if ( GOTOU == 0 ) { spriteB = 24; };
};


       if ( GOTOU == 0 && joueurB == 52 ) {
    BarbarianBodyX(g, 1U, 4U); yAttB = yGB; xAttB = xB + 4; yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline75, 1U);

    if ( GOTOU == 0 && temps > reftempsB + 20 ) {
        Mix_PlayChannel(2, BARB_SFX_MISS, 0); spriteB = 29;
        if ( GOTOU == 0 && joueurA == 52 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xAttB = xB - 1; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline76, 1U);

    if ( GOTOU == 0 ) { spriteB = 26; };
};

       if ( GOTOU == 0 && joueurB == 53 ) {
    BarbarianBodyX(g, 1U, 11U); yAttB = yGB; xAttB = xB; yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline77, 1U);

    if ( GOTOU == 0 && temps > reftempsB + 20 ) {
        Mix_PlayChannel(2, BARB_SFX_MISS, 0); spriteB = 29;
        if ( GOTOU == 0 && joueurA == 53 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xAttB = xB + 5; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline78, 1U);

    if ( GOTOU == 0 ) { spriteB = 26; };
};

       if ( GOTOU == 0 && joueurB == 54 ) {
    BarbarianBodyX(g, 1U, 0U); spriteB = 30; yAttB = yGB; yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline79, 1U);

};

       if ( GOTOU == 0 && joueurB == 55 ) {
    BarbarianBodyX(g, 1U, 15U); spriteB = 30; yAttB = yGB; yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline80, 1U);

};

       if ( GOTOU == 0 && joueurB == 56 ) {
    BarbarianBodyX(g, 1U, 0U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline81, 1U);

};

       if ( GOTOU == 0 && joueurB == 57 ) {
    BarbarianBodyX(g, 1U, 15U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline82, 1U);

};

       if ( GOTOU == 0 && joueurB == 58 ) {
decapiteB = 0; yAttB = yTB;
xFB = xB + 1; xTB = xB + 2; xMB = xB + 2; xGB = xB + 0;
BarbarianTimeline(g, &ctx, barbTimeline83, 1U);

};

       if ( GOTOU == 0 && joueurB == 59 ) {
decapiteB = 0; yAttB = yTB;
xFB = xB + 3; xTB = xB + 2; xMB = xB + 2; xGB = xB + 4;
BarbarianTimeline(g, &ctx, barbTimeline84, 1U);

};

       if ( GOTOU == 0 && joueurB == 60 ) {
    BarbarianBodyX(g, 1U, 0U); yGB = 20;
    BarbarianTimeline(g, &ctx, barbTimeline85, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 36;
        if ( GOTOU == 0 && joueurA == 60 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xFB = xB + 4; xAttB = xB - 3; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline86, 1U);

};

       if ( GOTOU == 0 && joueurB == 61 ) {
    yGB = 20; BarbarianBodyX(g, 1U, 15U);
    BarbarianTimeline(g, &ctx, barbTimeline87, 1U);

if ( GOTOU == 0 && temps > reftempsB + 13 ) {
        spriteB = 36;
        if ( GOTOU == 0 && joueurA == 61 ) {
            GOTOU = 2;
        };
        if ( GOTOU == 0 ) { xFB = xB; xAttB = xB + 7; GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline88, 1U);

};

      if ( GOTOU == 0 && joueurB == 62 ) {
    xAttB = xB + 4; BarbarianBodyX(g, 1U, 0U); yAttB = 14; spriteB = 37;
    BarbarianTimeline(g, &ctx, barbTimeline89, 1U);

};

       if ( GOTOU == 0 && joueurB == 63 ) {
    xAttB = xB; BarbarianBodyX(g, 1U, 15U); yAttB = 14; spriteB = 37;
    BarbarianTimeline(g, &ctx, barbTimeline90, 1U);

};


       if ( GOTOU == 0 && joueurB == 64 ) {
    spriteB = 40; decapiteB = 1; sangB = 0; xAttB = xB + 4; yAttB = 14; yGB = 20; BarbarianBodyX(g, 1U, 0U);
    BarbarianTimeline(g, &ctx, barbTimeline91, 1U);

};

       if ( GOTOU == 0 && joueurB == 65 ) {
    spriteB = 40; decapiteB = 1; sangB = 0; xAttB = xB; yAttB = 14; yGB = 20; BarbarianBodyX(g, 1U, 15U);
    BarbarianTimeline(g, &ctx, barbTimeline92, 1U);

};

       if ( GOTOU == 0 && joueurB == 66 ) {
    decapiteB = 1; sangB = 0; xAttB = xB + 4; yGB = 20; BarbarianBodyX(g, 1U, 0U);
    if ( GOTOU == 0 && temps > reftemps + 230 ) {
        gnome = 1; reftemps = temps; xGNOME = 1; xSPRt = 270; GOTOU = 31;
    };
    BarbarianTimeline(g, &ctx, barbTimeline93, 1U);

    if ( GOTOU == 0 && temps > reftempsB + 35 ) {
        distance = xB - xA;
        if ( GOTOU == 0 && distance < 5 ) {
            spriteB = 6; xB = xB + 1;
        };
        if ( GOTOU == 0 && distance > 5 ) {
            spriteB = 6; xB = xB - 1;
        };
        if ( GOTOU == 0 ) { GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline94, 1U);

};

       if ( GOTOU == 0 && joueurB == 67 ) {
    decapiteB = 1; sangB = 0; xAttB = xB; yGB = 20; BarbarianBodyX(g, 1U, 15U);
    if ( GOTOU == 0 && temps > reftemps + 230 ) {
        gnome = 1; reftemps = temps; xGNOME = 1; xSPRt = 270; GOTOU = 31;
    };
    BarbarianTimeline(g, &ctx, barbTimeline95, 1U);

    if ( GOTOU == 0 && temps > reftempsB + 35 ) {
        distance = xA - xB;
        if ( GOTOU == 0 && distance < 5 ) {
            spriteB = 6; xB = xB - 1;
        };
        if ( GOTOU == 0 && distance > 5 ) {
            spriteB = 6; xB = xB + 1;
        };
        if ( GOTOU == 0 ) { GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline96, 1U);

};

      ///                               ******degats B ******;
       if ( GOTOU == 0 && joueurB == 68 ) {
    attenteB = 0; xAttB = xB + 4; BarbarianBodyX(g, 1U, 15U); BarbarianBodyY(g, 1U);
    if ( GOTOU == 0 && joueurA == 54 ) {
        joueurB = 72; GOTOU = 24;
    };
    if ( GOTOU == 0 ) { sangB = 1; serpentB = 1; };
    if ( GOTOU == 0 && joueurA == 58 && decapiteB == 1 && spriteB != 9 ) {
#if BARB_PUBLIC
        joueurB = 78; vieB = 0;
#else
        joueurB = 80;
#endif
        Boccupe = 1; reftempsB = temps; xSPRt = xB + 3; BarbarianAwardHit(g,1,1); sangB = 0; GOTOU = 33;
    };
    if ( GOTOU == 0 ) { BarbarianAwardHit(g,1,0); vieB = vieB - 1; };
    if ( GOTOU == 0 && vieB <= 0 ) {
        joueurB = 78; Boccupe = 1; reftempsB = temps; sangB = 1; GOTOU = 33;
    };
    if(GOTOU==0)BarbarianHitSound(g,2);
if ( GOTOU == 0 ) { joueurB = 69; spriteB=44; xB=xB+2; Boccupe = 1; reftempsB = temps; decapiteB = 1; GOTOU = 2; };
    BARB_BLADE_HIT(1);
};

       if ( GOTOU == 0 && joueurB == 69 ) {
attenteB = 0; xAttB = xB+4;
BarbarianBodyX(g, 1U, 15U);
BarbarianTimeline(g, &ctx, barbTimeline97, 1U);

};

       if ( GOTOU == 0 && joueurB == 70 ) {
    attenteB = 0; xAttB = xB; BarbarianBodyX(g, 1U, 0U); BarbarianBodyY(g, 1U);
    if ( GOTOU == 0 && joueurA == 55 ) {
        joueurB = 74; GOTOU = 24;
    };
    if ( GOTOU == 0 ) { sangB = 1; serpentB = 1; };
    if ( GOTOU == 0 && joueurA == 59 && decapiteB == 1 && spriteB != 9 ) {
#if BARB_PUBLIC
        joueurB = 79; vieB = 0;
#else
        joueurB = 81;
#endif
        Boccupe = 1; reftempsB = temps; xSPRt = xB + 3; BarbarianAwardHit(g,1,1); sangB = 0; GOTOU = 33;
    };
    if ( GOTOU == 0 ) { BarbarianAwardHit(g,1,0); vieB = vieB - 1; };
    if ( GOTOU == 0 && vieB <= 0 ) {
        joueurB = 79; Boccupe = 1; reftempsB = temps; sangB = 1; GOTOU = 33;
    };
    if(GOTOU==0)BarbarianHitSound(g,2);
if ( GOTOU == 0 ) { joueurB = 71; spriteB=44; xB=xB-2; Boccupe = 1; reftempsB = temps; decapiteB = 1; GOTOU = 2; };
    BARB_BLADE_HIT(1);
};

       if ( GOTOU == 0 && joueurB == 71 ) {
attenteB = 0; xAttB = xB;
BarbarianBodyX(g, 1U, 0U);
BarbarianTimeline(g, &ctx, barbTimeline98, 1U);

};

       if ( GOTOU == 0 && joueurB == 72 ) {
    attenteB = 0; xAttB = xB + 4; BarbarianBodyX(g, 1U, 15U); BarbarianBodyY(g, 1U); sangB = 1; serpentB = 1; vieB = vieB - 1; BarbarianAwardHit(g,1,2);
    if ( GOTOU == 0 && joueurA == 32 ) {
        vieB = vieB + 1; sangB = 0; serpentB = 0;
    };
    if ( GOTOU == 0 && vieB <= 0 ) {
        joueurB = 78; Boccupe = 1; reftempsB = temps; sangB = 1;
        if ( GOTOU == 0 && (joueurA == 56 || joueurA == 54) ) {
            sangB = 0;
        };
        if ( GOTOU == 0 ) { GOTOU = 33; };
    };
    if ( GOTOU == 0 && joueurA == 56 ) {
         sangB = 0; Mix_PlayChannel(2, BARB_SFX_HEADHIT, 0);
    };
    if ( GOTOU == 0 && joueurA == 54 ) {
         sangB = 0; Mix_PlayChannel(2, BARB_SFX_FOOTCUT, 0);
    };
    if ( GOTOU == 0 ) { joueurB = 73; spriteB=45; xB=xB+2; Boccupe = 1; reftempsB = temps; GOTOU = 2; };
    BARB_BLADE_HIT(1);
};

       if ( GOTOU == 0 && joueurB == 73 ) {
attenteB = 0; xAttB = xB+4;
BarbarianBodyX(g, 1U, 15U);
if ( GOTOU == 0 && temps == reftempsB + 2 ) { BarbarianFallSound(g,1); };
BarbarianTimeline(g, &ctx, barbTimeline99, 1U);

};

       if ( GOTOU == 0 && joueurB == 74 ) {
    xAttB = xB; attenteB = 0; BarbarianBodyX(g, 1U, 0U); BarbarianBodyY(g, 1U); sangB = 1; serpentB = 1; vieB = vieB - 1; BarbarianAwardHit(g,1,2);
    if ( GOTOU == 0 && joueurA == 34 ) {
        vieB = vieB + 1; sangB = 0; serpentB = 0;
    };
    if ( GOTOU == 0 && vieB <= 0 ) {
        joueurB = 79; Boccupe = 1; reftempsB = temps; sangB = 1;
        if ( GOTOU == 0 && (joueurA == 57 || joueurA == 55) ) {
            sangB = 0;
        };
        if ( GOTOU == 0 ) { GOTOU = 33; };
    };
    if ( GOTOU == 0 && joueurA == 57 ) {
         sangB = 0; Mix_PlayChannel(2, BARB_SFX_HEADHIT, 0);
    };
    if ( GOTOU == 0 && joueurA == 55 ) {
         sangB = 0; Mix_PlayChannel(2, BARB_SFX_FOOTCUT, 0);
    };
    if ( GOTOU == 0 ) { joueurB = 75; spriteB=45; xB=xB-2; Boccupe = 1; reftempsB = temps; GOTOU = 2; };
    BARB_BLADE_HIT(1);
};

       if ( GOTOU == 0 && joueurB == 75 ) {
attenteB = 0; xAttB = xB;
BarbarianBodyX(g, 1U, 0U);
if ( GOTOU == 0 && temps == reftempsB + 2 ) { BarbarianFallSound(g,1); };
BarbarianTimeline(g, &ctx, barbTimeline100, 1U);

};


      //                               bruit des epees  et decapitations loupees;
       if ( GOTOU == 0 && sens == 6 ) {

           if ( GOTOU == 0 && joueurB == 76 ) {
    if ( GOTOU == 0 && joueurA == 58 && decapiteB == 0 ) {
        joueurB = 68; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
    if ( GOTOU == 0 && joueurA == 50 ) {
        joueurB = 68; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurB = 43; GOTOU = 2; };
};

           if ( GOTOU == 0 && joueurB == 77 ) {
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurB = 39; GOTOU = 2; };
};

      };

       if ( GOTOU == 0 && sens == 9 ) {

           if ( GOTOU == 0 && joueurB == 76 ) {
    if ( GOTOU == 0 && joueurA == 59 && decapiteB == 0 ) {
        joueurB = 70; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
    if ( GOTOU == 0 && joueurA == 51 ) {
        joueurB = 70; Boccupe = 1; reftempsB = temps; GOTOU = 24;
    };
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurB = 45; GOTOU = 2; };
};

           if ( GOTOU == 0 && joueurB == 77 ) {
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurB = 41; GOTOU = 2; };
};

      };

      ///                                   *************************************;
      ///                                   **********gestion de la mort*********;
      ///                                   *************************************;
      //mortB ;
       if ( GOTOU == 33 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurB == 78 ) {
    BarbarianBodyX(g, 1U, 0U);
    BarbarianTimeline(g, &ctx, barbTimeline101, 1U);

    if ( GOTOU == 0 ) { spriteB = 9; joueurA = 66; Aoccupe = 1; reftemps = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(2, BARB_SFX_DEATHFALL, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurB == 79 ) {
    BarbarianTimeline(g, &ctx, barbTimeline102, 1U);

    if ( GOTOU == 0 ) { spriteB = 9; joueurA = 67; Aoccupe = 1; reftemps = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(2, BARB_SFX_DEATHFALL, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurB == 80 ) {
    BarbarianTimeline(g, &ctx, barbTimeline103, 1U);

    if ( GOTOU == 0 ) { spriteB = 51; joueurA = 64; Aoccupe = 1; reftemps = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(2, BARB_SFX_DECAPITATED, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurB == 81 ) {
    BarbarianTimeline(g, &ctx, barbTimeline104, 1U);

    if ( GOTOU == 0 ) { spriteB = 51; joueurA = 65; Aoccupe = 1; reftemps = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(2, BARB_SFX_DECAPITATED, 0); lancerfin = 0;
    };
};


      ///                   ***************************************;
      ///                   ***********   SORCIER    **************;
      ///                   ***************************************;

       if ( GOTOU == 0 && joueurB == 82 ) {
    gnome = 0; BarbarianBodyX(g, 1U, 15U);
    BarbarianTimeline(g, &ctx, barbTimeline105, 1U);

    if ( GOTOU == 0 ) { spriteB = 55; xAttB = xB; yAttB = 16; yGB = 20; sangB = 0; };
};



    ///                   ***************************************;
    ///                   ***********   COLISION   **************;
    ///                   ***************************************;
    //colision ;
      if ( GOTOU == 24 && joueurB >= 32 && joueurB <= 82 ) { goto barb_management_B; }
if ( GOTOU == 2 ) { GOTOU = 0 ; }

     if ( GOTOU == 0 && sens == 9 ) { GOTOU = 3 ; }

     if ( GOTOU == 0 && joueurA == 24 && joueurB == 32 ) { GOTOU = 28 ; }
     if ( GOTOU == 0 && joueurB == 24 && joueurA == 32 ) { GOTOU = 28 ; }



     if ( GOTOU == 0 ) {

      //                               pour empecher que A entre dans B;

        if ( levier1 == 6 || joueurA == 32 || joueurA == 58 ) {
                distance = xB - xA;
         if ( distance < 4 ) { xA = xA - 1 ; }
        };

      //                                pour empecher que B entre dans A;

      if ( levier2 == 4 || joueurB == 32 || joueurB == 58 ) {
        distance = xB - xA;
        if ( distance < 4 ) { xB = xB + 1 ; }
     };


      //                               garder la distance debout;
      if ( joueurB == 0 && joueurA == 0 ) {
        distance = xB - xA;
         if ( distance < 4 ) { xA = xA - 1, xB = xB + 1 ; }
     };

    };

    //                               sortie du cadre;
    //sortiecadre ;
     if ( GOTOU == 28 ) { GOTOU = 0 ; }

     if ( GOTOU == 0 && entree == 1 ) {
         if ( xA < 0 ) { xA = 0 ; }
         if ( xB < 0 ) { xB = 0 ; }
         if ( xA > 40 ) { xA = 40 ; }
         if ( xB > 40 ) { xB = 40 ; }
        GOTOU = 30;
    };

     if ( GOTOU == 0 ) {
         if ( sortieA == 1 || sortieB == 1 ) {
             if ( xA < 0 ) { xA = 0 ; }
             if ( xB > 40 ) { xB = 40 ; }
            GOTOU = 30;
        };
    };

     if ( GOTOU == 0 && entree == 0 ) {
         if ( xA < 5 ) { xA = 5 ; }
         if ( xA > 32 ) { xA = 32 ; }
         if ( xB < 5 ) { xB = 5 ; }
         if ( xB > 32 ) { xB = 32 ; }
    };


     if ( GOTOU == 0 ) { GOTOU = 30 ; }


     ///                   ***************************************;
     ///                   ***********   COLISION INVERSE ********;
     ///                   ***************************************;

      if ( GOTOU == 0 && sens == 6 ) { GOTOU = 31 ; }

     //colisionR ;
      if ( GOTOU == 3 ) { GOTOU = 0 ; }

      if ( GOTOU == 0 && joueurA == 25 && joueurB == 34 ) { GOTOU = 27 ; }
      if ( GOTOU == 0 && joueurB == 25 && joueurA == 34 ) { GOTOU = 27 ; }

     //                               pour empecher que A entre dans B (inverse);

      if ( GOTOU == 0 ) {
          if ( levier1 == 4 || joueurA == 34 || joueurA == 59 ) {
             distance = xA - xB;
              if ( distance < 4 ) { xA = xA + 1 ; }
         };
     };


     ///                                pour empecher que B entre dans A (inverse);

      if ( GOTOU == 0 ) {
          if ( levier2 == 6 || joueurB == 34 || joueurB == 59 ) {
             distance = xA - xB;
              if ( distance < 4 ) { xB = xB - 1 ; }
         };
     };

     //                               garder la distance debout;
      if ( GOTOU == 0 && joueurB == 1 && joueurA == 1 ) {
         distance = xA - xB;
          if ( distance < 4 ) { xA = xA + 1, xB = xB - 1 ; }
     };


     //sortiecadreR ;
      if ( GOTOU == 27 ) { GOTOU = 0 ; }

      if ( GOTOU == 0 ) {
          if ( sortieA == 1 || sortieB == 1 || entreesorcier == 1 ) {
              if ( xB < 0 ) { xB = 0 ; }
              if ( xA > 40 ) { xA = 40 ; }
             GOTOU = 30;
         };
     };

      if ( GOTOU == 0 ) {
          if ( xA < 5 ) { xA = 5 ; }
          if ( xA > 31 ) { xA = 32 ; }
          if ( xB < 5 ) { xB = 5 ; }
          if ( xB > 32 ) { xB = 32 ; }
     };


     ///                       ****************************************;
     ///                       ********* les tetes volantes ! *********;
     ///                       ****************************************;
     //tetesvolantes ;
      if ( GOTOU == 30 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 ) {
           if ( sortieA == 1 || sortieB == 1 ) { GOTOU = 31 ; }
      };

       if ( GOTOU == 0 ) {

           if ( xA > 19 && xB > 20 ) { GOTOU = 29 ; }

           if ( GOTOU == 0 ) {
               if ( joueurA == 80 || joueurA == 81 ) {


                   BarbarianHeadFlight(g, &ctx, 0U);};
          };


           if ( GOTOU == 0 ) {
               if ( joueurB == 80 || joueurB == 81 ) {

                   BarbarianHeadFlight(g, &ctx, 1U);GOTOU = 31;
              };
          };

          //teteagauche ;
           if ( GOTOU == 29 ) { GOTOU = 0 ; }

           if ( GOTOU == 0 ) {
               if ( joueurA == 80 || joueurA == 81 ) {

                   BarbarianHeadFlight(g, &ctx, 2U);};
          };

           if ( GOTOU == 0 ) {
               if ( joueurB == 80 || joueurB == 81 ) {

                   BarbarianHeadFlight(g, &ctx, 3U);};
          };


      };


      ///                   *********************************;
