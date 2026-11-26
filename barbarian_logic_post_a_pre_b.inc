/* Extracted from original SDL main.c lines 1659-3587. Do not clean up until trace checks pass. */
     ///                       **********************************;
     ///                       ******* animations  auto *********;
     ///                       **********************************;

      if ( GOTOU == 0 && entree == 1 ) {
         if ( lancerintro == 1 ) {
             Mix_PlayChannel(1, prepare, 0);
             lancerintro = 0;
         }
         levier1 = 6; GOTOU = 25;
     };

      if ( GOTOU == 0 && sortieA == 1 ) {
          if ( sens == 9 ) { levier1 = 6, GOTOU = 25 ; }
          if ( GOTOU == 0 ) { sens = 6, levier1 = 4, GOTOU = 25 ; }
     };

      if(GOTOU==0&&entreesorcier==1){
    if(xA<=33){ entreesorcier=0;GOTOU=26; }
    if(GOTOU==0){ levier1=4;GOTOU=25; }
};

     ///                   ************************************;
     ///                   **********   MODE DEMO   ***********;
     ///                   ************************************;



       if ( GOTOU == 0 && demo == 1 ) {

           if ( sens == 6 ) {
              distance = xB - xA;

               if ( distance >= 15 ) { //  quand trop loin;
                  joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26;
              };


               if ( GOTOU == 0 && distance == 12 && joueurB == 0 ) { joueurA = 58, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }

               if ( GOTOU == 0 && distance == 9 ) {
                   if ( GOTOU == 0 && attenteB > 50 ) { joueurA = 58, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && joueurB == 36 ) { joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && Boccupe == 1 ) { joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && distance < 9 && distance > 6 ) { // distance de combat 1;

                  // pour se rapprocher;
                   if ( joueurB == 36 ) { joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && levier2 == 4 ) { joueurA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }

                   if ( GOTOU == 0 && joueurB == 60 ) { joueurA = 39, reftemps = temps, GOTOU = 26 ; }


                  //pour eviter les degats repetitifs;
                   if ( GOTOU == 0 && infoAdegatG > 4 ) {
                       if ( joueurB == 27 || joueurB == 50 ) { joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };
                   if ( GOTOU == 0 && infoAdegatG > 2 ) {
                       if ( joueurB == 27 || joueurB == 50 ) { joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                   if ( GOTOU == 0 && infoAdegatT > 2 ) {
                       if ( joueurB == 46 ) { joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                   if ( GOTOU == 0 && infoAdegatF > 2 ) {
                       if ( joueurB == 60 ) { joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                  ///pour alterner les attaques;
                  /// mode non aleatoire
                   //if ( GOTOU == 0 && infocoupA == 0 ) { joueurA = 48, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 1 ) { joueurA = 60, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 2 ) { joueurA = 52, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 3 ) { joueurA = 52, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 4 ) { joueurA = 46, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 5 ) { levier1 = 4, infocoupA = 0, GOTOU = 25 ; }
                ///mode aleatoire
                if ( GOTOU == 0 && infocoupA < 10 ) { infocoupA = rand()%60+1, joueurA = 48, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 10 && infocoupA < 20 ) { infocoupA = rand()%60+1, joueurA = 60, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 20 && infocoupA < 30 ) { infocoupA = rand()%60+1, joueurA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 30 && infocoupA < 40 ) { infocoupA = rand()%60+1, joueurA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 40 && infocoupA < 50 ) { infocoupA = rand()%60+1, joueurA = 46, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 50 ) { infocoupA = 0, levier1 = 4, GOTOU = 25 ; }


              };

               if ( GOTOU == 0 && distance <= 6 ) {

                   if ( joueurB == 48 ) { joueurA = 43, reftemps = temps, GOTOU = 26 ; }

                   if ( GOTOU == 0 && infoAdegatG > 4 ) {
                       if ( joueurB == 27 || joueurB == 50 ) { joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };
                   if ( GOTOU == 0 && infoAdegatG > 2 ) {
                       if ( joueurB == 54 ) { joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                       if ( GOTOU == 0 ) {
                        if ( joueurB == 27 || joueurB == 50 ) { joueurA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                        };
                  };

                   /// pour alterner les attaques
                   /// mode non aleatoire
                   //if ( GOTOU == 0 && infocoupA == 0 ) { joueurA = 54, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 1 ) { joueurA = 56, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 2 ) { joueurA = 52, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 3 ) { joueurA = 50, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 4 ) { joueurA = 50, infocoupA = infocoupA + 1, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   //if ( GOTOU == 0 && infocoupA == 5 ) { levier1 = 4, infocoupA = 0, GOTOU = 25 ; }
                ///mode aleatoire
                if ( GOTOU == 0 && infocoupA < 10 ) { infocoupA = rand()%60+1, joueurA = 54, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 10 && infocoupA < 20 ) { infocoupA = rand()%60+1, joueurA = 56, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 20 && infocoupA < 30 ) { infocoupA = rand()%60+1, joueurA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 30 && infocoupA < 40 ) { infocoupA = rand()%60+1, joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 40 && infocoupA < 50 ) { infocoupA = rand()%60+1, joueurA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 50 ) { infocoupA = 0, levier1 = 4, GOTOU = 25 ; }

              };


          };


           if ( GOTOU == 0 && sens == 9 ) { GOTOU = 8 ; }

      };





     ///                       redirection suivant les touches;

      if ( GOTOU == 0 && levier1 == 1 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 3 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 7 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 9 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 2 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 8 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 4 ) { GOTOU = 25 ; }
      if ( GOTOU == 0 && levier1 == 6 ) { GOTOU = 25 ; }


     ///                       action si aucune touche n//a ete touche;
      if ( GOTOU == 0 && (joueurA == 43 || joueurA == 45) ) { joueurA=sens+22; protegeDA=2; Aoccupe=1; reftemps=temps; GOTOU=26; };
if ( GOTOU == 0 ) {
         spriteavanceA = 0;
         spritereculeA = 0;

         protegeDA = 0, protegeHA = 0;

         attenteA = attenteA + 1;
         levier1 = 5 ;
     };

     /// pour se relever;
      if ( GOTOU == 0 ) { assisA = 0 ; }
      if ( GOTOU == 0 && joueurA == 27 ) { joueurA = 28, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
      if ( GOTOU == 0 && joueurA == 30 ) { joueurA = 31, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }


     ///attente des 5 secondes;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( attenteA > 300 ) { joueurA = 2, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
     };
      if ( GOTOU == 0 && sens == 9 ) {
          if ( attenteA > 300 ) { joueurA = 3, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
     };

     /// etat debout;
      if ( GOTOU == 0 && sens == 6 ) { joueurA = 0 ; }
      if ( GOTOU == 0 && sens == 9 ) { joueurA = 1 ; }


      if ( GOTOU == 0 ) { GOTOU = 26 ; }



     ///               *********************************************************;
     ///               ***************** ACTIONS suivant clavier ***************;
     ///               *********************************************************;

     //action ;
      if ( GOTOU == 25 ) { GOTOU = 0 ; }

      if ( GOTOU == 0 ) { attenteA = 1 ; } // remise a zero de l//attente


     //                       droite,gauche, decapite, devant (normal);


      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 6 ) {
             protegeDA = 0;
              if ( GOTOU == 0 && spriteavanceA == 1 ) { joueurA = 5, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 2 ) { joueurA = 6, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 3 ) { joueurA = 7, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 4 ) { joueurA = 8, GOTOU = 26 ; }
              if ( GOTOU == 0 && attaqueA == 1 && demo == 0 && entree == 0 ) { joueurA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 ) { joueurA = 4, reftemps = temps, GOTOU = 26 ; }
         };

          if ( GOTOU == 0 && levier1 == 4 ) {
             protegeHA = 0;
              if ( GOTOU == 0 && spritereculeA == 1 ) { joueurA = 15, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 2 ) { joueurA = 16, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 3 ) { joueurA = 17, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 4 ) { joueurA = 18, GOTOU = 26 ; }
              if ( GOTOU == 0 && attaqueA == 1 && demo == 0 && sortieA == 0 ) { joueurA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 ) { joueurA = 14, reftemps = temps, GOTOU = 26 ; }
         };
     };

      //                       droite, gauche, decapite, devant (inverse);

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier1 == 6 ) {
              protegeHA = 0;
               if ( GOTOU == 0 && spritereculeA == 1 ) { joueurA = 20, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 2 ) { joueurA = 21, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 3 ) { joueurA = 22, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 4 ) { joueurA = 23, GOTOU = 26 ; }
               if ( GOTOU == 0 && attaqueA == 1 && demo == 0 && sortieA == 0 ) { joueurA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 ) { joueurA = 19, reftemps = temps, GOTOU = 26 ; }
          };

           if ( GOTOU == 0 && levier1 == 4 ) {
              protegeDA = 0;
               if ( GOTOU == 0 && spriteavanceA == 1 ) { joueurA = 10, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 2 ) { joueurA = 11, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 3 ) { joueurA = 12, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 4 ) { joueurA = 13, GOTOU = 26 ; }
               if ( GOTOU == 0 && attaqueA == 1 && demo == 0 && entreesorcier == 0 ) { joueurA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 ) { joueurA = 9, reftemps = temps, GOTOU = 26 ; }
          };
      };


     //                           saute, attaque cou;

      if ( GOTOU == 0 && sens == 6 ) {
          if ( GOTOU == 0 && levier1 == 8 ) {
             protegeDA = 0, protegeHA = 0;
             joueurA = 24, Aoccupe = 1, reftemps = temps, GOTOU = 26;
         };
     };

      if ( GOTOU == 0 && sens == 9 ) {
          if ( levier1 == 8 ) {
             protegeDA = 0, protegeHA = 0;
             joueurA = 25, Aoccupe = 1, reftemps = temps, GOTOU = 26;
         };
     };


     //                               assis, attaque genou;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 2 ) {
              if ( attaqueA == 1 && demo == 0 ) { joueurA = 50, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( assisA == 1 ) { joueurA = 27, GOTOU = 26 ; }
              if ( GOTOU == 0 ) { joueurA = 26, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ) {
          if ( levier1 == 2 ) {
              if ( attaqueA == 1 && demo == 0 ) { joueurA = 51, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( assisA == 1 ) { joueurA = 30, GOTOU = 26 ; }
              if ( GOTOU == 0 ) { joueurA = 29, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };


     //                               roulade AV, coup de pied;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 9 ) {
              if ( attaqueA == 1 && demo == 0 ) { joueurA = 54, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ) { joueurA = 32, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ) {
          if ( levier1 == 9 ) {
             if ( attaqueA == 1 && demo == 0 ) { joueurA = 55, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ) { joueurA = 34, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };



     //                               roulade AR, coup sur front;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 7 ) {
              if ( attaqueA == 1 && demo == 0 ) { joueurA = 60, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ) { joueurA = 36, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ) {
          if ( levier1 == 7 ) {
              if ( attaqueA == 1 && demo == 0 ) { joueurA = 61, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ) { joueurA = 37, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };

     //                                   protection Haute, araignee;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 1 ) {
              if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 52, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 && protegeHA == 1 ) { joueurA = 39, GOTOU = 26 ; }
              if ( GOTOU == 0 ) { joueurA = 38, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };

       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier1 == 1 ) {
               if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 53, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 && protegeHA == 1 ) { joueurA = 41, GOTOU = 26 ; }
               if ( GOTOU == 0 ) { joueurA = 40, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          };
      };


     //                                   protection devant, coup de tete;
      if ( GOTOU == 0 && sens == 6 ) {
          if ( levier1 == 3 ) {
                if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 56, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
                if ( protegeDA == 1 ) { joueurA = 43, GOTOU = 26 ; }
                if ( GOTOU == 0 ) { joueurA = 42, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
       if ( GOTOU == 0 && sens == 9 ) {
           if ( levier1 == 3 ) {
               if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 57, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( protegeDA == 1 ) { joueurA = 45, GOTOU = 26 ; }
               if ( GOTOU == 0 ) { joueurA = 44, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          };
      };

     ///               ********************************************;
     ///               *************GESTION DES ETATS**************;
     ///               ********************************************;
     //gestion ;
      barb_management_A:;
if ( GOTOU == 26 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurA == 2 ) {
    BarbarianBodyX(g, 0U, 15U);
    if ( GOTOU == 0 && temps > reftemps + 50 ) {
        Aoccupe = 0; attenteA = 1; joueurA = 0; GOTOU = 1;
    };
    BarbarianTimeline(g, &ctx, barbTimeline0, 0U);

};


      if ( GOTOU == 0 && joueurA == 3 ) {
    BarbarianBodyX(g, 0U, 0U);
    if ( GOTOU == 0 && temps > reftemps + 50 ) {
        Aoccupe = 0; attenteA = 1; joueurA = 1; GOTOU = 1;
    };
    BarbarianTimeline(g, &ctx, barbTimeline1, 0U);

};


      if ( GOTOU == 0 && joueurA == 0 ) {
    spriteA = 0; decapiteA = 1; sangA = 0; xAttA = xA; yAttA = 14; BarbarianBodyY(g, 0U); BarbarianBodyX(g, 0U, 15U);
    if ( GOTOU == 0 && demo == 1 ) {
        if ( GOTOU == 0 && temps > reftemps + 20 ) {
            Aoccupe = 0;
        };
    };
};

      if ( GOTOU == 0 && joueurA == 1 ) {
    spriteA = 0; decapiteA = 1; sangA = 0; xAttA = xFA + 4; yAttA = 14; BarbarianBodyY(g, 0U); BarbarianBodyX(g, 0U, 0U);
};


     //                               avance;

      if ( GOTOU == 0 ) { BarbarianHandleMoveA(g, &GOTOU, temps, &reftemps); }

      if ( GOTOU == 0 ) { BarbarianHandleJump(g, &GOTOU, temps, &reftemps, 0U); };

      ;

       if ( GOTOU == 0 ) { BarbarianHandleCrouch(g, &ctx, 0U); };

       ;


       ;

       ;

       ;

       ;


       if ( GOTOU == 0 && joueurA == 32 ) {
    BarbarianBodyX(g, 0U, 0U); yGA  =  20; yAttA  =  yGA; xAttA  =  xA; yTA  =  yGA;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        yTA  =  16; joueurA  =  54; Aoccupe  =  1; reftemps  =  temps; GOTOU  =  26;
    };

BarbarianTimeline(g, &ctx, barbTimeline2, 0U);

};

      //finderoulade ;
       if ( GOTOU == 4 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurA == 33 ) {
    if ( GOTOU == 0 && xA < xB ) {
        joueurA = 0; spriteA = 0; xAttA = xA; yGA = 20; yAttA = 17; BarbarianBodyX(g, 0U, 15U); BarbarianBodyY(g, 0U); GOTOU = 1;
    };
    if ( GOTOU == 0 && xA > xB - 1 ) {
        joueurA = 62; yAttA = 14; Aoccupe = 1; reftemps = temps; joueurB = 62; yAttB = 14; Boccupe = 1; reftempsB = temps; GOTOU = 31;
    };
};

       if ( GOTOU == 0 && joueurA == 34 ) {
    BarbarianBodyX(g, 0U, 15U); yGA  =  20; yAttA  =  yGA; xAttA  =  xA + 4; yTA  =  yGA;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA  =  55; Aoccupe  =  1; reftemps  =  temps; GOTOU  =  26;
    };

BarbarianTimeline(g, &ctx, barbTimeline3, 0U);

};

      //finderouladeR ;
       if ( GOTOU == 5 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurA == 35 ) {
    if ( GOTOU == 0 && xA > xB ) {
        joueurA = 1; spriteA = 0; xAttA = xA + 4; yGA = 20; yAttA = 17; BarbarianBodyX(g, 0U, 0U); BarbarianBodyY(g, 0U); GOTOU = 1;
    };
    if ( GOTOU == 0 && xA < xB + 1 ) {
        joueurA = 63; yAttA = 14; Aoccupe = 1; reftemps = temps; joueurB = 63; yAttB = 14; Boccupe = 1; reftempsB = temps; GOTOU = 31;
    };
};


       if ( GOTOU == 0 && joueurA == 36 ) {
    BarbarianBodyX(g, 0U, 0U); yGA  =  20; yAttA  =  yGA; xAttA  =  xA;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA  =  60; Aoccupe  =  1; reftemps  =  temps; GOTOU  =  26;
    };

BarbarianTimeline(g, &ctx, barbTimeline4, 0U);

};


       if ( GOTOU == 0 && joueurA == 37 ) {
    BarbarianBodyX(g, 0U, 15U); yGA  =  20; yAttA  =  yGA; xAttA  =  xA + 4;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA  =  61; Aoccupe  =  1; reftemps  =  temps; GOTOU  =  26;
    };

BarbarianTimeline(g, &ctx, barbTimeline5, 0U);

};


          if ( GOTOU == 0 && joueurA == 38 ) {
BarbarianBodyX(g, 0U, 15U);
xAttA = xA; yGA = 20;
if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 52; Aoccupe = 1; reftemps = temps; GOTOU = 26; };
BarbarianTimeline(g, &ctx, barbTimeline6, 0U);

};

        if ( GOTOU == 0 && joueurA == 39 ) {
    BarbarianBodyX(g, 0U, 15U); xAttA = xA; yGA = 20; spriteA = 17;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA = 52; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
};

       if ( GOTOU == 0 && joueurA == 40 ) {
BarbarianBodyX(g, 0U, 0U);
xAttA = xA + 4; yGA = 20;
if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 53; Aoccupe = 1; reftemps = temps; GOTOU = 26; };
BarbarianTimeline(g, &ctx, barbTimeline7, 0U);

};

       if ( GOTOU == 0 && joueurA == 41 ) {
    BarbarianBodyX(g, 0U, 0U); xAttA = xA + 4; yGA = 20; spriteA = 17;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA = 53; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
};


       if ( GOTOU == 0 && joueurA == 42 ) {
BarbarianBodyX(g, 0U, 15U);
xAttA = xA; yGA = 20;
if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 56; Aoccupe = 1; reftemps = temps; GOTOU = 26; };
decapiteA = 1;
BarbarianTimeline(g, &ctx, barbTimeline8, 0U);

};

       if ( GOTOU == 0 && joueurA == 43 ) {
    xAttA = xA; yGA = 20; BarbarianBodyX(g, 0U, 15U); decapiteA = 0; spriteA = 18;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA = 56; Aoccupe = 1; reftemps = temps;
    };
};

       if ( GOTOU == 0 && joueurA == 44 ) {
BarbarianBodyX(g, 0U, 0U);
xAttA = xA + 4; yGA = 20;
if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) { joueurA = 57; Aoccupe = 1; reftemps = temps; GOTOU = 26; };
decapiteA = 1;
BarbarianTimeline(g, &ctx, barbTimeline9, 0U);

};

       if ( GOTOU == 0 && joueurA == 45 ) {
    xAttA = xA + 4; yGA = 20; BarbarianBodyX(g, 0U, 0U); decapiteA = 0; spriteA = 18;
    if ( GOTOU == 0 && attaqueA == 1 && demo == 0 ) {
        joueurA = 57; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
};

       if ( GOTOU == 0 && joueurA == 46 ) {
    BarbarianBodyX(g, 0U, 15U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline10, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 20;
        if ( GOTOU == 0 && joueurB == 46 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xTA = xA; xAttA = xA + 7; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline11, 0U);

};

       if ( GOTOU == 0 && joueurA == 47 ) {
    BarbarianBodyX(g, 0U, 0U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline12, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 20;
        if ( GOTOU == 0 && joueurB == 47 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xTA = xA + 4; xAttA = xA - 3; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline13, 0U);

};

       if ( GOTOU == 0 && joueurA == 48 ) {
    BarbarianBodyX(g, 0U, 15U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline14, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 23;
        if ( GOTOU == 0 && joueurB == 48 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xMA = xA; xAttA = xA + 6; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline15, 0U);

    if ( GOTOU == 0 ) { spriteA = 21; };
};

       if ( GOTOU == 0 && joueurA == 49 ) {
    BarbarianBodyX(g, 0U, 0U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline16, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 23;
        if ( GOTOU == 0 && joueurB == 49 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xMA = xA + 4; xAttA = xA - 2; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline17, 0U);

    if ( GOTOU == 0 ) { spriteA = 21; };
};

       if ( GOTOU == 0 && joueurA == 50 ) {
    BarbarianBodyX(g, 0U, 8U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline18, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 25;
        if ( GOTOU == 0 && joueurB == 50 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xGA = xA; xAttA = xA + 7; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline19, 0U);

    if ( GOTOU == 0 ) { spriteA = 24; };
};

       if ( GOTOU == 0 && joueurA == 51 ) {
    yGA = 20; BarbarianBodyX(g, 0U, 7U);
    BarbarianTimeline(g, &ctx, barbTimeline20, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 25;
        if ( GOTOU == 0 && joueurB == 51 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xGA = xA + 4; xAttA = xA - 3; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline21, 0U);

    if ( GOTOU == 0 ) { spriteA = 24; };
};

       if ( GOTOU == 0 && joueurA == 52 ) {
    BarbarianBodyX(g, 0U, 11U); yAttA = yGA; xAttA = xA; yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline22, 0U);

    if ( GOTOU == 0 && temps > reftemps + 20 ) {
        Mix_PlayChannel(1, BARB_SFX_MISS, 0); spriteA = 29;
        if ( GOTOU == 0 && joueurB == 52 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xAttA = xA + 5; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline23, 0U);

    if ( GOTOU == 0 ) { spriteA = 26; };
};

       if ( GOTOU == 0 && joueurA == 53 ) {
    BarbarianBodyX(g, 0U, 4U); yAttA = yGA; xAttA = xA + 4; yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline24, 0U);

    if ( GOTOU == 0 && temps > reftemps + 20 ) {
        Mix_PlayChannel(1, BARB_SFX_MISS, 0); spriteA = 29;
        if ( GOTOU == 0 && joueurB == 53 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xAttA = xA - 1; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline25, 0U);

    if ( GOTOU == 0 ) { spriteA = 26; };
};

       if ( GOTOU == 0 && joueurA == 54 ) {
    BarbarianBodyX(g, 0U, 15U); spriteA = 30; yAttA = yGA; yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline26, 0U);

};

       if ( GOTOU == 0 && joueurA == 55 ) {
    BarbarianBodyX(g, 0U, 0U); spriteA = 30; yAttA = yGA; yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline27, 0U);

};

       if ( GOTOU == 0 && joueurA == 56 ) {
    BarbarianBodyX(g, 0U, 15U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline28, 0U);

    if ( GOTOU == 0 ) { spriteA = 32; };
};

       if ( GOTOU == 0 && joueurA == 57 ) {
    BarbarianBodyX(g, 0U, 0U); yGA = 20; yTA = 16;
    BarbarianTimeline(g, &ctx, barbTimeline29, 0U);

    if ( GOTOU == 0 ) { spriteA = 32; };
};

       if ( GOTOU == 0 && joueurA == 58 ) {
decapiteA = 0; yAttA = yTA;
xFA = xA + 3; xTA = xA + 2; xMA = xA + 2; xGA = xA + 4;
BarbarianTimeline(g, &ctx, barbTimeline30, 0U);

};

       if ( GOTOU == 0 && joueurA == 59 ) {
decapiteA = 0; yAttA = yTA;
xFA = xA + 1; xTA = xA + 2; xMA = xA + 2; xGA = xA + 0;
BarbarianTimeline(g, &ctx, barbTimeline31, 0U);

};

       if ( GOTOU == 0 && joueurA == 60 ) {
    BarbarianBodyX(g, 0U, 15U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline32, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 36;
        if ( GOTOU == 0 && joueurB == 60 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xFA = xA; xAttA = xA + 7; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline33, 0U);

};

       if ( GOTOU == 0 && joueurA == 61 ) {
    BarbarianBodyX(g, 0U, 0U); yGA = 20;
    BarbarianTimeline(g, &ctx, barbTimeline34, 0U);

if ( GOTOU == 0 && temps > reftemps + 13 ) {
        spriteA = 36;
        if ( GOTOU == 0 && joueurB == 61 ) {
            if ( GOTOU == 0 && distance < 12 ) {
                BarbarianClashSound(g);
            };
            if ( GOTOU == 0 ) { GOTOU = 1; };
        };
        if ( GOTOU == 0 ) { xFA = xA + 4; xAttA = xA - 3; GOTOU = 1; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline35, 0U);

};

      if ( GOTOU == 0 && joueurA == 62 ) {
    xAttA = xA; BarbarianBodyX(g, 0U, 15U); yAttA = 14; spriteA = 37;
    BarbarianTimeline(g, &ctx, barbTimeline36, 0U);

};

       if ( GOTOU == 0 && joueurA == 63 ) {
    xAttA = xA + 4; BarbarianBodyX(g, 0U, 0U); yAttA = 14; spriteA = 37;
    BarbarianTimeline(g, &ctx, barbTimeline37, 0U);

};

       if ( GOTOU == 0 && joueurA == 64 ) {
    spriteA = 40; decapiteA = 1; sangA = 0; xAttA = xA; yGA = 20; yAttA = 14; BarbarianBodyX(g, 0U, 15U);
    BarbarianTimeline(g, &ctx, barbTimeline38, 0U);

};

       if ( GOTOU == 0 && joueurA == 65 ) {
    spriteA = 40; decapiteA = 1; sangA = 0; xAttA = xFA + 4; yGA = 20; yAttA = 14; BarbarianBodyX(g, 0U, 0U);
    BarbarianTimeline(g, &ctx, barbTimeline39, 0U);

};

       if ( GOTOU == 0 && joueurA == 66 ) {
    decapiteA = 1; sangA = 0; xAttA = xA; yGA = 20; BarbarianBodyX(g, 0U, 15U);
    if ( GOTOU == 0 && temps > reftemps + 230 ) {
        gnome = 1; reftemps = temps; xGNOME = 1; xSPRt = 270; GOTOU = 31;
    };
    BarbarianTimeline(g, &ctx, barbTimeline40, 0U);

    if ( GOTOU == 0 && temps > reftemps + 35 ) {
        distance = xB - xA;
        if ( GOTOU == 0 && distance < 5 ) {
            spriteA = 6; xA = xA - 1;
        };
        if ( GOTOU == 0 && distance > 5 ) {
            spriteA = 6; xA = xA + 1;
        };
        if ( GOTOU == 0 ) { GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline41, 0U);

};

       if ( GOTOU == 0 && joueurA == 67 ) {
    decapiteA = 1; sangA = 0; xAttA = xFA + 4; yGA = 20; BarbarianBodyX(g, 0U, 0U);
    if ( GOTOU == 0 && temps > reftemps + 230 ) {
        gnome = 1; reftemps = temps; xGNOME = 1; xSPRt = 270; GOTOU = 31;
    };
    BarbarianTimeline(g, &ctx, barbTimeline42, 0U);

    if ( GOTOU == 0 && temps > reftemps + 35 ) {
        distance = xA - xB;
        if ( GOTOU == 0 && distance < 5 ) {
            spriteA = 6; xA = xA + 1;
        };
        if ( GOTOU == 0 && distance > 5 ) {
            spriteA = 6; xA = xA - 1;
        };
        if ( GOTOU == 0 ) { GOTOU = 2; };
    };
    BarbarianTimeline(g, &ctx, barbTimeline43, 0U);

};


      //                               ******degats******;
       if ( GOTOU == 0 && joueurA == 68 ) {
    attenteA = 0; xAttA = xA; BarbarianBodyX(g, 0U, 0U); BarbarianBodyY(g, 0U);
    if ( GOTOU == 0 && joueurB == 54 ) {
        joueurA = 72; GOTOU = 26;
    };
    if ( GOTOU == 0 ) { sangA = 1; serpentA = 1; };
    if ( GOTOU == 0 && joueurB == 58 && decapiteA == 1 && spriteA != 9 ) {
#if BARB_PUBLIC
        joueurA = 78; vieA = 0;
#else
        joueurA = 80;
#endif
        Aoccupe = 1; reftemps = temps; xSPRt = xA + 3; BarbarianAwardHit(g,0,1); sangA = 0; GOTOU = 34;
    };
    if ( GOTOU == 0 ) { BarbarianAwardHit(g,0,0); vieA = vieA - 1; };
    if ( GOTOU == 0 && vieA <= 0 ) {
        joueurA = 78; Aoccupe = 1; reftemps = temps; sangA = 1; GOTOU = 34;
    };
    if(GOTOU==0)BarbarianHitSound(g,1);
if ( GOTOU == 0 ) { joueurA = 69; spriteA=44; xA=xA-2; Aoccupe = 1; reftemps = temps; decapiteA = 1; GOTOU = 1; };
    BARB_BLADE_HIT(0);
};


       if ( GOTOU == 0 && joueurA == 69 ) {
attenteA = 0; xAttA = xA;
BarbarianBodyX(g, 0U, 0U);
BarbarianTimeline(g, &ctx, barbTimeline44, 0U);

};


       if ( GOTOU == 0 && joueurA == 70 ) {
    attenteA = 0; xAttA = xA + 4; BarbarianBodyX(g, 0U, 15U); BarbarianBodyY(g, 0U);
    if ( GOTOU == 0 && joueurB == 55 ) {
        joueurA = 74; GOTOU = 26;
    };
    if ( GOTOU == 0 ) { sangA = 1; serpentA = 1; };
    if ( GOTOU == 0 && joueurB == 59 && decapiteA == 1 && spriteA != 9 ) {
#if BARB_PUBLIC
        joueurA = 79; vieA = 0;
#else
        joueurA = 81;
#endif
        Aoccupe = 1; reftemps = temps; xSPRt = xA + 3; BarbarianAwardHit(g,0,1); sangA = 0; GOTOU = 34;
    };
    if ( GOTOU == 0 ) { BarbarianAwardHit(g,0,0); vieA = vieA - 1; };
    if ( GOTOU == 0 && vieA <= 0 ) {
        joueurA = 79; Aoccupe = 1; reftemps = temps; sangA = 1; GOTOU = 34;
    };
    if(GOTOU==0)BarbarianHitSound(g,1);
if ( GOTOU == 0 ) { joueurA = 71; spriteA=44; xA=xA+2; Aoccupe = 1; reftemps = temps; decapiteA = 1; GOTOU = 1; };
    BARB_BLADE_HIT(0);
};

       if ( GOTOU == 0 && joueurA == 71 ) {
attenteA = 0; xAttA = xA+4;
BarbarianBodyX(g, 0U, 15U);
BarbarianTimeline(g, &ctx, barbTimeline45, 0U);

};


       if ( GOTOU == 0 && joueurA == 72 ) {
    xAttA = xA; attenteA = 0; BarbarianBodyX(g, 0U, 0U); BarbarianBodyY(g, 0U); sangA = 1; serpentA = 1; vieA = vieA - 1; BarbarianAwardHit(g,0,2);
    if ( GOTOU == 0 && joueurB == 32 ) {
        vieA = vieA + 1; sangA = 0; serpentA = 0;
    };
    if ( GOTOU == 0 && vieA <= 0 ) {
        joueurA = 78; Aoccupe = 1; reftemps = temps; sangA = 1;
        if ( GOTOU == 0 && (joueurB == 56 || joueurB == 54) ) {
            sangA = 0;
        };
        if ( GOTOU == 0 ) { GOTOU = 34; };
    };
    if ( GOTOU == 0 && joueurB == 56 ) {
         sangA = 0; Mix_PlayChannel(1, BARB_SFX_HEADHIT, 0);
    };
    if ( GOTOU == 0 && joueurB == 54 ) {
         sangA = 0; Mix_PlayChannel(1, BARB_SFX_FOOTCUT, 0);
    };
    if ( GOTOU == 0 ) { joueurA = 73; spriteA=45; xA=xA-2; Aoccupe = 1; reftemps = temps; GOTOU = 1; };
    BARB_BLADE_HIT(0);
};


       if ( GOTOU == 0 && joueurA == 73 ) {
attenteA = 0; xAttA = xA;
BarbarianBodyX(g, 0U, 0U);
if ( GOTOU == 0 && temps == reftemps + 2 ) { BarbarianFallSound(g,0); };
BarbarianTimeline(g, &ctx, barbTimeline46, 0U);

};


       if ( GOTOU == 0 && joueurA == 74 ) {
    xAttA = xA + 4; attenteA = 0; BarbarianBodyX(g, 0U, 15U); BarbarianBodyY(g, 0U); sangA = 1; serpentA = 1; vieA = vieA - 1; BarbarianAwardHit(g,0,2);
    if ( GOTOU == 0 && joueurB == 34 ) {
        vieA = vieA + 1; sangA = 0; serpentA = 0;
    };
    if ( GOTOU == 0 && vieA <= 0 ) {
        joueurA = 79; Aoccupe = 1; reftemps = temps; sangA = 1;
        if ( GOTOU == 0 && (joueurB == 57 || joueurB == 55) ) {
            sangA = 0;
        };
        if ( GOTOU == 0 ) { GOTOU = 34; };
    };
    if ( GOTOU == 0 && joueurB == 57 ) {
        Mix_PlayChannel(1, BARB_SFX_HEADHIT, 0);  sangA = 0;
    };
    if ( GOTOU == 0 && joueurB == 55 ) {
        Mix_PlayChannel(1, BARB_SFX_FOOTCUT, 0);  sangA = 0;
    };
    if ( GOTOU == 0 ) { joueurA = 75; spriteA=45; xA=xA+2; Aoccupe = 1; reftemps = temps; GOTOU = 1; };
    BARB_BLADE_HIT(0);
};

       if ( GOTOU == 0 && joueurA == 75 ) {
attenteA = 0; xAttA = xA+4;
BarbarianBodyX(g, 0U, 15U);
if ( GOTOU == 0 && temps == reftemps + 2 ) { BarbarianFallSound(g,0); };
BarbarianTimeline(g, &ctx, barbTimeline47, 0U);

};



      //                               bruit des epees  et decapitation loupee;
       if ( GOTOU == 0 && sens == 6 ) {

           if ( GOTOU == 0 && joueurA == 76 ) {
    if ( GOTOU == 0 && joueurB == 58 && decapiteA == 0 ) {
        joueurA = 68; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
    if ( GOTOU == 0 && joueurB == 50 ) {
        joueurA = 68; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurA = 43; GOTOU = 1; };
};

           if ( GOTOU == 0 && joueurA == 77 ) {
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurA = 39; GOTOU = 1; };
};

      };

       if ( GOTOU == 0 && sens == 9 ) {

           if ( GOTOU == 0 && joueurA == 76 ) {
    if ( GOTOU == 0 && joueurB == 59 && decapiteA == 0 ) {
        joueurA = 70; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
    if ( GOTOU == 0 && joueurB == 51 ) {
        joueurA = 70; Aoccupe = 1; reftemps = temps; GOTOU = 26;
    };
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurA = 45; GOTOU = 1; };
};

           if ( GOTOU == 0 && joueurA == 77 ) {
    if ( GOTOU == 0 && distance < 12 ) {
        BarbarianClashSound(g);
    };
    if ( GOTOU == 0 ) { joueurA = 41; GOTOU = 1; };
};

      };



     ///                                   *************************************;
     ///                                   **********gestion de la mort*********;
     ///                                   *************************************;

     //mort ;
      if ( GOTOU == 34 ) { GOTOU = 0 ; }

       if ( GOTOU == 0 && joueurA == 78 ) {
    BarbarianTimeline(g, &ctx, barbTimeline48, 0U);

    if ( GOTOU == 0 ) { spriteA = 9; joueurB = 66; Boccupe = 1; reftempsB = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(1, BARB_SFX_DEATHFALL, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurA == 79 ) {
    BarbarianTimeline(g, &ctx, barbTimeline49, 0U);

    if ( GOTOU == 0 ) { spriteA = 9; joueurB = 67; Boccupe = 1; reftempsB = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(1, BARB_SFX_DEATHFALL, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurA == 80 ) {
    BarbarianTimeline(g, &ctx, barbTimeline50, 0U);

    if ( GOTOU == 0 ) { spriteA = 51; joueurB = 64; Boccupe = 1; reftempsB = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(1, BARB_SFX_DECAPITATED, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurA == 81 ) {
    BarbarianTimeline(g, &ctx, barbTimeline51, 0U);

    if ( GOTOU == 0 ) { spriteA = 51; joueurB = 65; Boccupe = 1; reftempsB = temps; };
    if ( GOTOU == 0 && lancerfin == 1 ) {
        Mix_PlayChannel(1, BARB_SFX_DECAPITATED, 0); lancerfin = 0;
    };
};

       if ( GOTOU == 0 && joueurA == 82 ) {
    BarbarianTimeline(g, &ctx, barbTimeline52, 0U);

    if ( GOTOU == 0 ) { spriteA = 9; };
};



     ///                         **************************************;
     ///                         *********     joueur 2    ************;
     ///                         **************************************;
     //debut joueur 2;
      if ( GOTOU == 26 && joueurA >= 32 && joueurA <= 82 ) { goto barb_management_A; }
if ( GOTOU == 1 ) { GOTOU = 0 ; }


      if ( GOTOU == 0 ) {
                if(GOTOU==0&&sorcier==1){
    sangB=0;
    if(xA<=xB+4){
        joueurB=85;joueurA=84;spriteB=57;spriteA=42;
        xB=13;xA=20;Boccupe=1;Aoccupe=1;reftempsB=temps;jeu=2;GOTOU=35;
    }
    if(GOTOU==0&&Boccupe==1){GOTOU=24;}
    if(GOTOU==0){sangB=0;GOTOU=21;}
}
         ; }

      if ( GOTOU == 0 && sortieA == 1 ) {
          if ( joueurB == 78 || joueurB == 80 || joueurB == 79 || joueurB == 81 ) { GOTOU = 2 ; }
     };

      if ( GOTOU == 0 && sortieB == 1 ) { GOTOU = 21 ; }


      //                           ************degats sur joueurB************;
if ( GOTOU == 0 && ((u8)(spriteB-43)<5 || spriteB==59) ) { GOTOU=24; };

       if(GOTOU==0){ u8 contact=BarbarianContact(g,1); if(contact){if(!(contact&64))BarbarianClashSound(g);joueurB=contact;GOTOU=24;} }
if ( GOTOU == 0 && Boccupe == 1 ) { GOTOU = 24 ; } // important pour les animations
      if ( GOTOU == 0 ) { sangB = 0 ; }

     ///                       ********************************************;
