#include "Barbarian.h"



void SwordCling(){
       if (++choixCLING > 2 ){ choixCLING = 0 ; }
                   PlayADPCM(3, SFX_CLING1+choixCLING, 0)
}


int main(){
	GetPrngNumber(GetTrueRandomSeed());
	if(GetPrngNumber(0) == 0)
		GetPrngNumber(0xACE);

	InitMusicPlayer(patches);
	SetMasterVolume(224);

}




///********************************************************************
///*                                                                  *
///*                                                                  *
///*                            BARBARIAN                             *
///*                                                                  *
///*                        palace software 1988                      *
///*                                                                  *
///*  remake of AMIGA 500 version by Francois Lecornec in 2016-2018   *
///*                                                                  *
///*                  http://barbarian.1987.free.fr                   *
///*                                                                  *
///*                                                                  *
///* to compil with SDL 1.2 +SDL_image+ SDL_ttf + SDL_gfx+ SDL_mixer  *
///*                                                                  *
///*                                                                  *
///*  thanks to alain leguirec, openclassroom and devellopez.com      *
///*                         for the help !!!                         *
///*                                                                  *
///********************************************************************

    /// *************** declaration des variables ***************

    int temps = 0;
    int reftemps = 1;
    int reftempsB = 1;
    int chrono = 1;
    int ySPRt = 111;
    int xSPRt = 3;
    uint16_t score[2];

    int attenteA = 1;
    int attenteB = 1;
    int refserpent[0] = 0;
    int refserpent[1] = 0;	

    char GOTOU = 100;

    char pA = 0;
    char pB = 0;	
    char partie=0;
    char demo=0;
    char timer = 0;
    char xA = 0;
    char xB = 37;
    char yAttA = 17;
    char xAttA = 15;
    char yAttB = 17;
    char xAttB = 27;
    char yFA = 15;
    char yFB = 15;
    char yTA = 16;
    char yTB = 16;
    char yMA = 18;
    char yMB = 18;
    char yGA = 20;
    char yGB = 20;
    char xFA = xA + 4;
    char xTA = xA + 4;
    char xMA = xA + 4;
    char xGA = xA + 4;
    char xFB = xB;
    char xTB = xB;
    char xMB = xB;
    char xGB = xB;
    char sens = 6;
    uint8_t decapitated[2];
    decapitated[0] = 1;
    decapitated[1] = 1;
    uint8_t life[2];
    life[0] = 12;
    life[1] = 12;

    char entrance = 1 ;
    char Aoccupe = 0;
    char Boccupe = 0;
    char spriteA = 0;
    char spriteB = 0;
    char spriteavanceA = 0;
    char spritereculeA = 0;
    char spriteavanceB = 0;
    char spritereculeB = 0;
    uint8_t blood[2];
    blood[0] = 0;
    blood[1] = 0;
    char positionSANG = 0;
    uint8_t serpent[2];
    serpent[0] = 0;
    serpent[1] = 0;

	uint8_t bloodDecap[2];
   bloodDecap[0] = 0;
   bloodDecap[1] = 0;
    char gnome = 0;
    char xGNOME = 1;
    char numerotete = 1;
    char numerognome = 1;
    char football = 0;
    char sortieA = 0;
    char sortieB = 0;
    char protegeDA = 0;
    char protegeDB = 0;
    char protegeHA = 0;
    char protegeHB = 0;
    char tempsfini = 0;
    char gnometire = 1;
    uint8_t bonus[2];
    bonus[0] = 0;
    bonus[1] = 0;
    char infoBdegatF = 0;
    char infoBdegatT = 0;
    char infoBdegatG = 0;
    char infocoupA = 0;
    char infocoupB = 0;
    char infoAdegatF = 0;
    char infoAdegatT = 0;
    char infoAdegatG = 0;
    char jeu = 1;
    char enterSorceror = 0;
    int8_t attack[2];
    attack[0] = 0;
    attack[1] = 0;
    char transition = 0;
    char transitionfinie = 0;
    //char afficheFINAL = 0;
    char choixCLING = 0;
    char choixTOUCHE = 0;
    char songrogne = 0;
    char lancerintro = 0;
    char version = 0;
    char assisA = 0;
    char assisB = 0;
    char arret = 0;
    char distance = 0;
    char decorS = 2;
    char IA = 4;
    char sorceror = 0;
    char marianna = 0;
    char x1 = 0;
    char y1 = 0;
    int8_t lever[2];
    level[0] = 0;
    level[1] = 0;
    char x2 = 0;
    char y2 = 0;
    char couleurok = 0;

    char texteIA[10] = "";
    char texteSCORE[10] = "";



/// variables de lancement du jeu;

	timer = 60;

   /// demo;
   partie = 1, decorS = 2, IA = 4, sorceror = 0, marianna = 0, demo = 1;


   /// 1 p;
   //partie = 1, decorS = 2, IA = 0, sorceror = 0, marianna = 0, demo = 0;
   //partie = 1, decorS = 4, IA = 7, sorceror = 0, marianna = 0, demo = 0; // sorceror;

   /// mode vs;
   //partie = 2, decorS = 2, IA = 0, sorceror = 0, marianna = 0, demo = 0;

GOTOU = 100;






/// *************** Main Loop ******************
MAIN_TOP:
	WaitVsync(1);

     if(SDL_GetTicks() > (checkTime + 1000 / fps) ) // limitation a 60fps
     {
        checkTime = SDL_GetTicks();




    if ( GOTOU == 100 ){ // display title screen for a few seconds

	temps = temps + 1;
	if (temps >= 200 ){ GOTOU = 8 ; }
    };


///menu ;
      if ( GOTOU == 8){


    imageB = deboutB;

        GOTOU = 16;

     };


     ///               *********************************************************;
     ///               *****************    variables       ********************;
     ///               *********************************************************;

     //variables ;
     if ( GOTOU == 16 ){

     GOTOU = 0;

    xA = 0, xB = 37;
    yAttA = 17, xAttA = 15;
    yAttB = 17, xAttB = 27;
    yFA = 15, yFB = 15 ;
    yTA = 16, yTB = 16 ;
    yMA = 18, yMB = 18 ;
    yGA = 20, yGB = 20 ;
    xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
    xFB = xB, xTB = xB, xMB = xB, xGB = xB;
    sens = 6;
    pA = 0;
    pB = 0;
    decapitated[0] = 1;
    decapitated[1] = 1;
    life[0] = 12;
    life[1] = 12;
    temps = 1;
    reftemps = 1;
    attenteA = 1;
    attenteB = 1;
    entrance = 1 ;
    Aoccupe = 0;
    Boccupe = 0;
    spriteavanceA = 0;
    spritereculeA = 0;
    spriteavanceB = 0;
    spritereculeB = 0;
    blood[0] = 0;
    blood[1] = 0;
    positionSANG = 0;
    serpent[0] = 0;
    serpent[1] = 0;
    refserpent[0] = 0;
    refserpent[1] = 0;
    bloodDecap[0] = 0;
    bloodDecap[1] = 0;
    chrono = 1;
    ySPRt = 111;
    xSPRt = xA + 3;
    gnome = 0;
    xGNOME = 1;
    numerotete = 1;
    numerognome = 1;
    football = 0;
    sortieA = 0;
    sortieB = 0;
    protegeDA = 0;
    protegeDB = 0;
    protegeHA = 0;
    protegeHB = 0;
    tempsfini = 0;
    gnometire = 1;
    bonus[0] = 0;
    bonus[1] = 0;
    infoBdegatF = 0;
    infoBdegatT = 0;
    infoBdegatG = 0;
    infocoupA = 0;
    infocoupB = 0;
    infoAdegatF = 0;
    infoAdegatT = 0;
    infoAdegatG = 0;
    jeu = 1;
    enterSorceror = 0;
    attack[0] = 0;
    attack[1] = 0;


    transition = 0;
    transitionfinie = 0;



    choixCLING = 0;
    choixTOUCHE = 0;
    songrogne = 0;

    lancerintro = 0;
    version = 0;
    assisA = 0;
    assisB = 0;
    arret = 0;



     };



//TODO GET INPUT

              version = 0, arret = 0;

              if (keys[SDLK_RETURN])
            {
                    arret = 1;
            };

        if (keys[SDLK_v]){ version = 1 ;} // message cach\E9



      if ( GOTOU == 0 ){ temps = temps + 1 ; }



      ///                    ************* scenes d animation************;


       if ( GOTOU == 0 && jeu == 2 ){ GOTOU = 23 ; }


       if ( GOTOU == 0 && sorceror == 1 ){
           if ( xA <= 10 ){

               Mix_HaltChannel(2);

              sens = 6;

              Aoccupe = 1;
              pA = 83;
              spriteA = 42; // pA fini

              Boccupe = 1;
              pB = 85, marianna = 1, spriteB = 57;

              //xAttB=40;
              xA = 21, xB = 16;

              jeu = 2;

              GOTOU = 31;

          };

          GOTOU = 23;
      };


       if ( GOTOU == 0 && bonus[0] == 1 ){
          score[0] = score[0] + 10, timer = timer - 1;
           if ( timer < 1 ){
              timer = 0, score[0] = score[0] - 10;
               if ( xB >= 39 ){ sortieA = 1, Aoccupe = 0 ; }
          };
      };

       if ( GOTOU == 0 && bonus[1] == 1 ){
          score[1] = score[1] + 10, timer = timer - 1;
           if ( timer < 1 ){
              timer = 0, score[1] = score[1] - 10;
               if ( xA >= 39 ){ sortieB = 1, Boccupe = 0 ; }
          };
      };
      


      if ( GOTOU == 0 && entrance == 1 ){
          if ( xA >= 13 || xB <= 22 ){ entrance = 0 ; }
     };

      if ( GOTOU == 0 && sortieA == 1 ){

          if ( pB == 81 || pB == 79 ){
              if ( xA >= 39 ){

                  ///******* solo round wins in reverse?
                   if ( partie == 1 ){
                       if ( demo == 1 ){
						   //halt all sounds
                           if ( transition == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                           if ( GOTOU == 0 && transitionfinie == 1 ){ score[0] = 0, score[1] = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                      };

                       if ( GOTOU == 0 && transition == 1 ){ GOTOU = 26 ; }

                       if ( GOTOU == 0 && transitionfinie == 1 ){ IA = IA + 1 ; }

                       if ( GOTOU == 0 && IA > 7 ){
                          IA = 7, sorceror = 1;
                          sens = 9;
                          pB = 82, pA = 1;
                          xA = 35, xB = 6;
                          entrance = 0, sortieA = 0;
                          enterSorceror = 1;
                          Boccupe = 1 ;
                          reftempsB = temps;
                          //couleurok = 0;
                          GOTOU = 26;
                      };
                      //change background
                       if ( GOTOU == 0 ){
                           if ( IA == 0 ){ decorS = 2 ; }
                           if ( IA == 1 ){ decorS = 1 ; }
                           if ( IA == 2 ){ decorS = 2 ; }
                           if ( IA == 3 ){ decorS = 1 ; }
                           if ( IA == 4 ){ decorS = 3 ; }
                           if ( IA == 5 ){ decorS = 4 ; }
                           if ( IA == 6 ){ decorS = 3 ; }
                           if ( IA == 7 ){ decorS = 4 ; }

                           if ( demo == 1 ){ decorS = 2 ; }
                      };

                       if ( GOTOU == 0 && transitionfinie == 1 ){ GOTOU = 8 ; }

                       if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 26 ; }
                       if ( GOTOU == 0 ){ GOTOU = 8 ; }
                  };


                 ///******* round VS gagne en inverse;
                  if ( GOTOU == 0 && partie == 2 ){
                     if ( GOTOU == 0 && transition ==1 ){ GOTOU = 26 ; }
                      if ( GOTOU == 0 && transitionfinie == 1 ){
                         timer = 60;
						 //halt all sounds
                         if ( decorS == 2 ){ decorS = 1, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 1 ){ decorS = 3, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 3 ){ decorS = 4, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 && decorS == 4 ){ decorS = 2, GOTOU = 16 ; }// ex32
                         if ( GOTOU == 0 ){ GOTOU = 16 ; }// ex32
                     };
                      if ( GOTOU == 0 ){ transition = 1, reftemps = temps, GOTOU = 26 ; }
                 };
             };
         };



          if ( GOTOU == 0 && tempsfini == 0 ){
             if ( xA <= 1 && xB >= 39 ){
                  ///******* round solo gagne;
                   if ( partie == 1 ){
                       if ( demo == 1 ){
                            //halt all sounds
                           if ( transition == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                           if ( GOTOU == 0 && transitionfinie == 1 ){ score[0] = 0, score[1] = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                      };

                       if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }

                       if ( GOTOU == 0 && transitionfinie == 1 ){ IA = IA + 1 ; }

                       if ( GOTOU == 0 && IA > 7 ){
                          IA = 7, sorceror = 1;
                          sens = 9;
                          pB = 82, pA = 1;
                          xA = 35, xB = 6;
                          entrance = 0, sortieA = 0;
                          enterSorceror = 1;
                          Boccupe = 1 ;
                          reftempsB = temps;
                          //couleurok = 0;
                          GOTOU = 26;
                      };
                      /// changement de decor;
                       if ( GOTOU == 0 ){
                           if ( IA == 0 ){ decorS = 2 ; }
                           if ( IA == 1 ){ decorS = 1 ; }
                           if ( IA == 2 ){ decorS = 2 ; }
                           if ( IA == 3 ){ decorS = 1 ; }
                           if ( IA == 4 ){ decorS = 3 ; }
                           if ( IA == 5 ){ decorS = 4 ; }
                           if ( IA == 6 ){ decorS = 3 ; }
                           if ( IA == 7 ){ decorS = 4 ; }

                           if ( demo == 1 ){ decorS = 2 ; }
                      };

                       if ( GOTOU == 0 && transitionfinie == 1 ){ GOTOU = 8 ; }

                       if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                       if ( GOTOU == 0 ){ GOTOU = 8 ; }
                  };


                 ///******* round VS gagne;
                  if ( GOTOU == 0 && partie == 2 ){

                      if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }
                      if ( GOTOU == 0 && transitionfinie == 1 ){
                        timer = 60;
						//halt all sounds
                        if ( decorS == 2 ){ decorS = 1, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 1 ){ decorS = 3, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 3 ){ decorS = 4, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 && decorS == 4 ){ decorS = 2, GOTOU = 16 ; }// ex32
                        if ( GOTOU == 0 ){ GOTOU = 16 ; }// ex32
                     };

                      if ( GOTOU == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }

                 };
             };
        };

       if ( GOTOU == 0 && tempsfini == 1 ){
              if ( sens == 6 ){
                  if ( xA < 1 && xB >= 39 ){
                     timer = 60;
                     GOTOU = 16;
                  };
              };
              if ( GOTOU == 0 && sens == 9 ){
                   if ( xB < 1 && xA >= 39 ){
                     timer = 60;
                     GOTOU = 16;
                   };
              };

         };

          if ( GOTOU == 0 ){ GOTOU = 22 ; }
     };


       if ( GOTOU == 0 && sortieB == 1 ){
           if ( xB < 1 ){
               if ( pA == 81 || pA == 79 ){

                  ///********** solo game lost by reverse death
                    if ( partie == 1 ){
                        if ( demo == 1 ){
							//halt all sounds
                            if ( transition == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                            if ( GOTOU == 0 && transitionfinie == 1 ){ score[0] = 0, score[1] = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                       };

                        if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }
                        if ( GOTOU == 0 && transitionfinie == 1 ){
                           IA = 0, score[0] = 0, score[1] = 0;
                           decorS = 2, GOTOU = 8;
                       };

                        if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }

                        if ( GOTOU == 0 ){ GOTOU = 8 ; }

                   };

                  ///******* round VS lost in reverse
                   if ( GOTOU == 0 && partie == 2 ){
                       if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }
                       if ( GOTOU == 0 && transitionfinie == 1 ){
                          timer = 60;
						   //halt all sounds
                           if ( decorS == 2 ){ decorS = 1, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 1 ){ decorS = 3, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 3 ){ decorS = 4, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 4 ){ decorS = 2, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 ){ GOTOU = 16 ; }// ex32
                      };
                       if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }

                   };
              };
          };



           if ( GOTOU == 0 && tempsfini == 0 ){
               if ( xA >= 39 && xB >= 39 ){
                  ///********** solo party lost
                    if ( partie == 1 ){
                        if ( demo == 1 ){
							//halt all sounds
                            if ( transition == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                            if ( GOTOU == 0 && transitionfinie == 1 ){ score[0] = 0, score[1] = 0, temps = 0 , GOTOU = 100 ; } // ex goto 8
                       };

                        if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }
                        if ( GOTOU == 0 && transitionfinie == 1 ){
                           IA = 0, score[0] = 0, score[1] = 0;
                           decorS = 2, GOTOU = 8;
                       };

                        if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }
                        if ( GOTOU == 0 ){ GOTOU = 8 ; }
                   };

                  ///******* round VS perdu;
                   if ( GOTOU == 0 && partie == 2 ){
                       if ( GOTOU == 0 && transition == 1 ){ GOTOU = 31 ; }
                       if ( GOTOU == 0 && transitionfinie == 1 ){
                          timer = 60;
							//halt all sounds
                           if ( decorS == 2 ){ decorS = 1, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 1 ){ decorS = 3, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 3 ){ decorS = 4, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 && decorS == 4 ){ decorS = 2, GOTOU = 16 ; }// ex32
                           if ( GOTOU == 0 ){ GOTOU = 16 ; }// ex32
                      };
                       if ( GOTOU == 0 && sorceror == 0 ){ transition = 1, reftemps = temps, GOTOU = 31 ; }

                  };
              };
          };

           if ( GOTOU == 0 ){ GOTOU = 21 ; }

      };

    if ( GOTOU == 0 && gnome == 1 ){ GOTOU = 12 ; }



     ///                   ************************************;
     ///                   *******Damage Management***********;
     ///                   ************************************;

      if ( GOTOU == 23 ){ GOTOU = 0 ; }


     //damage to p1;

      if ( GOTOU == 0 && sorceror == 1 ){

           if ( xA < 31 ){
               if ( xAttB >= xTA && xAttB <= xA + 2 ){
                   if ( yAttB == yTA ){
                      gnome = 0;
                       if ( jeu == 0 ){ GOTOU = 26 ; }
                       if ( GOTOU == 0 ){ pA = 82, Aoccupe = 1, reftemps = temps, blood[0] = 0 ; }
                       if ( GOTOU == 0 ){ pB = 83, spriteB = 55, Boccupe = 1, reftempsB = temps, blood[1] = 0 ; } // sorcerorfini
                       if ( GOTOU == 0 ){ jeu = 0, GOTOU = 26 ; }
                  };
              };

               if ( GOTOU == 0 && xAttB >= xGA ){
                   if ( yAttB == yGA ){
                      gnome = 0;
                       if ( jeu == 0 ){ GOTOU = 26 ; }
                       if ( GOTOU == 0 ){ pA = 82, Aoccupe = 1, reftemps = temps, blood[0] = 0 ; }
                       if ( GOTOU == 0 ){ pB = 83, spriteB = 55, Boccupe = 1, reftempsB = temps, blood[1] = 0 ; }// sorcerorfini
                       if ( GOTOU == 0 ){ jeu = 0, GOTOU = 26 ; }
                  };
              };
          };

           if ( GOTOU == 0 && Aoccupe == 1 ){ GOTOU = 26 ; }
           if ( GOTOU == 0 ){ blood[0] = 0, GOTOU = 22 ; }

     };


      if ( GOTOU == 0 && sens == 6 ){
           if ( xA < xB ){
               if ( xAttB <= xFA && yAttB == yFA ){
                    positionSANG = yFA;
                   if ( pA == 39 ){ pA = 77, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 72, infoAdegatF = infoAdegatF + 1, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && xAttB <= xTA && yAttB == yTA ){
                     positionSANG = yTA;
                   if ( pB == 56 ){ pA = 72, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 68, score[1] = score[1] + 250, infoAdegatT = infoAdegatT + 1, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && xAttB <= xMA && yAttB == yMA ){
                     positionSANG = yMA;
                   if ( pA == 43 ){ pA = 76, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 68, score[1] = score[1] + 250, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && xAttB <= xGA && yAttB == yGA ){
                     positionSANG = yGA;
                   if ( pB == 52 ){ pA = 72, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pB == 32 ){ pA = 72, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pA == 43 ){ pA = 76, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pB == 54 ){ pA = 72, infoAdegatG = infoAdegatG + 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 68, score[1] = score[1] + 100, infoAdegatG = infoAdegatG + 1, GOTOU = 26 ; }
              };
          };

     };

       if ( GOTOU == 0 && sens == 9 ){
           if ( xA > xB ){
               if ( xAttB >= xFA && yAttB == yFA ){
                     positionSANG = yFA;
                   if ( pA == 41 ){ pA = 77, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 74, infoAdegatF = infoAdegatF + 1, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && xAttB >= xTA && yAttB == yTA ){
                     positionSANG = yTA;
                   if ( pB == 57 ){ pA = 74, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 70, score[1] = score[1] + 250, infoAdegatT = infoAdegatT + 1, GOTOU = 26 ; }
              };
               if ( GOTOU == 0 && xAttB >= xMA && yAttB == yMA ){
                     positionSANG = yMA;
                   if ( pA == 45 ){ pA = 76, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 70, score[1] = score[1] + 250, GOTOU = 26 ; }
              };
               if ( GOTOU == 0 && xAttB >= xGA && yAttB == yGA ){
                     positionSANG = yGA;
                   if ( pB == 53 ){ pA = 74, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pB == 34 ){ pA = 74, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pA == 45 ){ pA = 76, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pB == 55 ){ pA = 74, infoAdegatG = infoAdegatG + 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 ){ pA = 70, score[1] = score[1] + 100, infoAdegatG = infoAdegatG + 1, GOTOU = 26 ; }
              };
          };
      };



      if ( GOTOU == 0 && Aoccupe == 1 ){ GOTOU = 26 ; } // important for animations
      if ( GOTOU == 0 ){ blood[0] = 0 ; }


     ///               *********************************************************;
     ///               ***************** Input Events ********************;
     ///               *********************************************************;


     //clavier ;
      if ( GOTOU == 22 ){ GOTOU = 0 ; }



     ///                       reset the virtual keypad
      if ( GOTOU == 0 ){

         x1 = 7, y1 = 7;
         level[0] = 5; // neutral

          if ( demo == 0 ){
             //                       key entry;



            if (keys[SDLK_UP])
                y1 = y1 - 1 ;

            if (keys[SDLK_DOWN])
                y1 = y1 + 1 ;

            if (keys[SDLK_LEFT])
                x1 = x1 - 1 ;

            if (keys[SDLK_RIGHT])
                x1 = x1 + 1 ;


             //attack key

             attack[0] = 0;

              if (keys[SDLK_LSHIFT] || keys[SDLK_RSHIFT])
            {
                    attack[0] = 1;
            }




             //                      amplitude of the virtual block
              if ( y1 > 9 ){ y1 = 9 ; }
              if ( y1 < 5 ){ y1 = 5 ; }
              if ( x1 > 9 ){ x1 = 9 ; }
              if ( x1 < 5 ){ x1 = 5 ; }


             //                        position of the virtual lever

              if ( sens == 6 ){
                  if ( x1 == 6 && y1 == 6 ){ level[0] = 1 ; }
                 // hautG;
                  if ( x1 == 8 && y1 == 6 ){ level[0] = 3 ; }
                 // hautD;
                  if ( x1 == 6 && y1 == 8 ){ level[0] = 7 ; }
                 //basG;
                  if ( x1 == 8 && y1 == 8 ){ level[0] = 9 ; }
                 //basD;
                  if ( x1 == 6 && y1 == 7 ){ level[0] = 4 ; }
                 //gauche;
                  if ( x1 == 8 && y1 == 7 ){ level[0] = 6 ; }
                 //droite;
                  if ( x1 == 7 && y1 >= 8 ){ level[0] = 2 ; }
                 //bas;
                  if ( x1 == 7 && y1 == 6 ){ level[0] = 8 ; }
                 //haut;
             };
         };

          if ( sens == 9 ){
              if ( x1 <= 6 && y1 <= 6 ){ level[0] = 3 ; }
              if ( x1 >= 8 && y1 <= 6 ){ level[0] = 1 ; }
              if ( x1 <= 6 && y1 >= 8 ){ level[0] = 9 ; }
              if ( x1 >= 8 && y1 >= 8 ){ level[0] = 7 ; }
              if ( x1 <= 6 && y1 == 7 ){ level[0] = 4 ; }
              if ( x1 >= 8 && y1 == 7 ){ level[0] = 6 ; }
              if ( x1 == 7 && y1 >= 8 ){ level[0] = 2 ; }
              if ( x1 == 7 && y1 <= 6 ){ level[0] = 8 ; }
         };
     };
     ///                       **********************************;
     ///                       ******* animations  auto *********;
     ///                       **********************************;

      if ( GOTOU == 0 && entrance == 1 ){
         if ( lancerintro == 0 ){
                lancerintro = 1 ;
                PlayAPCM(1, prepare, 0);
                if ( serpent[0] == 0 && xA <= 2 ){ serpent[0] = 1, refserpent[0] = temps, serpent[1] = 1, refserpent[1] = temps ;}
         };

         level[0] = 6, GOTOU = 25;
     };

      if ( GOTOU == 0 && sortieA == 1 ){
          if ( sens == 9 ){ level[0] = 6, GOTOU = 25 ; }
          if ( GOTOU == 0 ){ sens = 6, level[0] = 4, GOTOU = 25 ; }
     };

      if ( GOTOU == 0 && enterSorceror == 1 ){
          if ( serpent[0] == 0 && xA >= 32 ){ serpent[0] = 1, refserpent[0] = temps, serpent[1] = 1, refserpent[1] = temps ;}
          
     /////     if ( GOTOU == 0 && xA <= 29 ){ enterSorceror = 0, GOTOU = 26 ; }
       ////   if ( GOTOU == 0 ){ level[0] = 4, GOTOU = 25 ; }
          if(GOTOU == 0){
          	if(xA <= 29){
          		enterSorceror = 0;
          		GOTOU = 26;
          	}else{
          		level[0] = 4;
          		GOTOU = 25;
          	}
          }
     };

     ///                   ************************************;
     ///                   **********   MODE DEMO   ***********;
     ///                   ************************************;



       if ( GOTOU == 0 && demo == 1 ){

           if ( sens == 6 ){
              distance = xB - xA;

               if ( distance >= 15 ){ //  quand trop loin;
                  pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26;
              };


               if ( GOTOU == 0 && distance == 12 && pB == 0 ){ pA = 58, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }

               if ( GOTOU == 0 && distance == 9 ){
                   if ( GOTOU == 0 && attenteB > 50 ){ pA = 58, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && pB == 36 ){ pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && Boccupe == 1 ){ pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
              };

               if ( GOTOU == 0 && distance < 9 && distance > 6 ){ // distance de combat 1;

                  // pour se rapprocher;
                   if ( pB == 36 ){ pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                   if ( GOTOU == 0 && level[1] == 4 ){ pA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }

                   if ( GOTOU == 0 && pB == 60 ){ pA = 39, reftemps = temps, GOTOU = 26 ; }


                  //pour eviter les degats repetitifs;
                   if ( GOTOU == 0 && infoAdegatG > 4 ){
                       if ( pB == 27 || pB == 50 ){ pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };
                   if ( GOTOU == 0 && infoAdegatG > 2 ){
                       if ( pB == 27 || pB == 50 ){ pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                   if ( GOTOU == 0 && infoAdegatT > 2 ){
                       if ( pB == 46 ){ pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                   if ( GOTOU == 0 && infoAdegatF > 2 ){
                       if ( pB == 60 ){ pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };

                  ///alternate attacks(random mode)

                if ( GOTOU == 0 && infocoupA < 10 ){ infocoupA = rand()%60+1, pA = 48, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 10 && infocoupA < 20 ){ infocoupA = rand()%60+1, pA = 60, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 20 && infocoupA < 30 ){ infocoupA = rand()%60+1, pA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 30 && infocoupA < 40 ){ infocoupA = rand()%60+1, pA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 40 && infocoupA < 50 ){ infocoupA = rand()%60+1, pA = 46, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 50 ){ infocoupA = 0, level[0] = 4, GOTOU = 25 ; }


              };

               if ( GOTOU == 0 && distance <= 6 ){

                   if ( pB == 48 ){ pA = 43, reftemps = temps, GOTOU = 26 ; }

                   if ( GOTOU == 0 && infoAdegatG > 4 ){
                       if ( pB == 27 || pB == 50 ){ pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                  };
                   if ( GOTOU == 0 && infoAdegatG > 2 ){
                       if ( pB == 54 ){ pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                       if ( GOTOU == 0 ){
			if ( pB == 27 || pB == 50 ){ pA = 32, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
			};
                  };

		///alternate attacks(random mode)

                if ( GOTOU == 0 && infocoupA < 10 ){ infocoupA = rand()%60+1, pA = 54, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 10 && infocoupA < 20 ){ infocoupA = rand()%60+1, pA = 56, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 20 && infocoupA < 30 ){ infocoupA = rand()%60+1, pA = 52, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 30 && infocoupA < 40 ){ infocoupA = rand()%60+1, pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 40 && infocoupA < 50 ){ infocoupA = rand()%60+1, pA = 50, reftemps = temps, Aoccupe = 1, GOTOU = 26 ; }
                if ( GOTOU == 0 && infocoupA >= 50 ){ infocoupA = 0, level[0] = 4, GOTOU = 25 ; }

              };


          };


           if ( GOTOU == 0 && sens == 9 ){ GOTOU = 8 ; }

      };





     ///                       redirection following keys

      if ( GOTOU == 0 && level[0] == 1 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 3 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 7 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 9 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 2 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 8 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 4 ){ GOTOU = 25 ; }
      if ( GOTOU == 0 && level[0] == 6 ){ GOTOU = 25 ; }


     ///                       action if no key has been touched
      if ( GOTOU == 0 ){
         spriteavanceA = 0;
         spritereculeA = 0;

         protegeDA = 0, protegeHA = 0;

         attenteA = attenteA + 1;
         level[0] = 5 ;
     };

     /// pour se relever;
      if ( GOTOU == 0 ){ assisA = 0 ; }
      if ( GOTOU == 0 && pA == 27 ){ pA = 28, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
      if ( GOTOU == 0 && pA == 30 ){ pA = 31, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }


     ///attente des 5 secondes;
      if ( GOTOU == 0 && sens == 6 ){
          if ( attenteA > 300 ){ pA = 2, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
     };
      if ( GOTOU == 0 && sens == 9 ){
          if ( attenteA > 300 ){ pA = 3, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
     };

     /// etat debout;
      if ( GOTOU == 0 && sens == 6 ){ pA = 0 ; }
      if ( GOTOU == 0 && sens == 9 ){ pA = 1 ; }


      if ( GOTOU == 0 ){ GOTOU = 26 ; }



     ///               *********************************************************;
     ///               ***************** Keyboard Actions ***************;
     ///               *********************************************************;

     //action ;
      if ( GOTOU == 25 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 ){ attenteA = 1 ; } // reset the wait


     //                      right, left, decapitated, front (normal);


      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 6 ){
             protegeDA = 0;
              if ( GOTOU == 0 && spriteavanceA == 1 ){ pA = 5, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 2 ){ pA = 6, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 3 ){ pA = 7, GOTOU = 26 ; }
              if ( GOTOU == 0 && spriteavanceA == 4 ){ pA = 8, GOTOU = 26 ; }
              if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 ){ pA = 4, reftemps = temps, GOTOU = 26 ; }
         };

          if ( GOTOU == 0 && level[0] == 4 ){
             protegeHA = 0;
              if ( GOTOU == 0 && spritereculeA == 1 ){ pA = 15, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 2 ){ pA = 16, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 3 ){ pA = 17, GOTOU = 26 ; }
              if ( GOTOU == 0 && spritereculeA == 4 ){ pA = 18, GOTOU = 26 ; }
              if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 ){ pA = 14, reftemps = temps, GOTOU = 26 ; }
         };
     };

      //                       right, left, decapitated, front (reverse);

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[0] == 6 ){
              protegeHA = 0;
               if ( GOTOU == 0 && spritereculeA == 1 ){ pA = 20, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 2 ){ pA = 21, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 3 ){ pA = 22, GOTOU = 26 ; }
               if ( GOTOU == 0 && spritereculeA == 4 ){ pA = 23, GOTOU = 26 ; }
               if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 ){ pA = 19, reftemps = temps, GOTOU = 26 ; }
          };

           if ( GOTOU == 0 && level[0] == 4 ){
              protegeDA = 0;
               if ( GOTOU == 0 && spriteavanceA == 1 ){ pA = 10, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 2 ){ pA = 11, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 3 ){ pA = 12, GOTOU = 26 ; }
               if ( GOTOU == 0 && spriteavanceA == 4 ){ pA = 13, GOTOU = 26 ; }
               if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 ){ pA = 9, reftemps = temps, GOTOU = 26 ; }
          };
      };


     //                           jumps, neck attack;

      if ( GOTOU == 0 && sens == 6 ){
          if ( GOTOU == 0 && level[0] == 8 ){
             protegeDA = 0, protegeHA = 0;
             pA = 24, Aoccupe = 1, reftemps = temps, GOTOU = 26;
         };
     };

      if ( GOTOU == 0 && sens == 9 ){
          if ( level[0] == 8 ){
             protegeDA = 0, protegeHA = 0;
             pA = 25, Aoccupe = 1, reftemps = temps, GOTOU = 26;
         };
     };


     //                               sitting, knee attack;


      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 2 ){
              if ( attack[0] == 1 && demo == 0 ){ pA = 50, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( assisA == 1 ){ pA = 27, GOTOU = 26 ; }
              if ( GOTOU == 0 ){ pA = 26, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ){
          if ( level[0] == 2 ){
              if ( attack[0] == 1 && demo == 0 ){ pA = 51, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( assisA == 1 ){ pA = 30, GOTOU = 26 ; }
              if ( GOTOU == 0 ){ pA = 29, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };


     //                               forward roll, kick;

      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 9 ){
              if ( attack[0] == 1 && demo == 0 ){ pA = 54, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ){ pA = 32, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ){
          if ( level[0] == 9 ){
	     if ( attack[0] == 1 && demo == 0 ){ pA = 55, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ){ pA = 34, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };



     //                               rear roll, blow to forehead;
      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 7 ){
              if ( attack[0] == 1 && demo == 0 ){ pA = 60, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ){ pA = 36, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
      if ( GOTOU == 0 && sens == 9 ){
          if ( level[0] == 7 ){
              if ( attack[0] == 1 && demo == 0 ){ pA = 61, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
             if ( GOTOU == 0 ){ pA = 37, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };

     //                                   High protection, web
      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 1 ){
              if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 52, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 && protegeHA == 1 ){ pA = 39, GOTOU = 26 ; }
              if ( GOTOU == 0 ){ pA = 38, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[0] == 1 ){
               if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 53, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 && protegeHA == 1 ){ pA = 41, GOTOU = 26 ; }
               if ( GOTOU == 0 ){ pA = 40, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          };
      };


     //                                   protection in front, headbutt;
      if ( GOTOU == 0 && sens == 6 ){
          if ( level[0] == 3 ){
		if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 56, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
		if ( protegeDA == 1 ){ pA = 43, GOTOU = 26 ; }
              	if ( GOTOU == 0 ){ pA = 42, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         };
     };
       if ( GOTOU == 0 && sens == 9 ){
           if ( level[0] == 3 ){
               if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 57, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( protegeDA == 1 ){ pA = 45, GOTOU = 26 ; }
               if ( GOTOU == 0 ){ pA = 44, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          };
      };

     ///               ********************************************;
     ///               *************State Management**************;
     ///               ********************************************;
     //gestion ;
      if ( GOTOU == 26 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pA == 2 ){ // attente
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
           if ( GOTOU == 0 && temps > reftemps + 100 ){ Aoccupe = 0, attenteA = 1, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 74 ){ spriteA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 60 ){ spriteA = 2, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 46 ){ spriteA = 3, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 2, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){ spriteA = 1, GOTOU = 1 , PlayAPCM(1, attente, 0); }
          GOTOU = 1;
      };


      if ( GOTOU == 0 && pA == 3 ){ // attenteR
         xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          if ( GOTOU == 0 && temps > reftemps + 100 ){ Aoccupe = 0, attenteA = 1, pA = 1, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 74 ){ spriteA = 1, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 60 ){ spriteA = 2, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 46 ){ spriteA = 3, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 2, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 20 ){ GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 19 ){  spriteA = 1, GOTOU = 1 , PlayAPCM(1, attente, 0); }
         GOTOU = 1;
     };


      if ( GOTOU == 0 && pA == 0 ){ // debout
         spriteA = 0;
         decapitated[0] = 1;
         blood[0] = 0;
         xAttA = xA, yAttA = 14;
         yFA = 15, yTA = 16, yMA = 18, yGA = 20;
         xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          if ( GOTOU == 0 && demo == 1 ){
              if ( GOTOU == 0 && temps > reftemps + 10 ){ Aoccupe = 0 ; }
         };
         GOTOU = 1;
     };

      if ( GOTOU == 0 && pA == 1 ){ // deboutR
         spriteA = 0;
         decapitated[0] = 1;
         blood[0] = 0;
         xAttA = xFA + 4, yAttA = 14;
         yFA = 15, yTA = 16, yMA = 18, yGA = 20;
         xFA = xA, xTA = xA, xMA = xA, xGA = xA;
         GOTOU = 1;
     };


     //                               avance;

      if ( GOTOU == 0 && pA == 4 ){ //avance
          if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          if ( GOTOU == 0 ){ xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4, xAttA = xA ; }
          if ( GOTOU == 0 ){ spriteA = 4, spriteavanceA = 1, xA = xA + 1, GOTOU = 1 ; }
     };

     if ( GOTOU == 0 && pA == 5 ){ // avance1
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 ){ xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4, xAttA = xA ; }
         if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA + 1, spriteA = 5, spriteavanceA = 2, GOTOU = 1 ; }
    };


     if ( GOTOU == 0 && pA == 6 ){ //avance2
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 ){ xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4, xAttA = xA, spriteA = 5 ; }
         if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA + 1, spriteA = 6, spriteavanceA = 3, GOTOU = 1 ; }
    };

     if ( GOTOU == 0 && pA == 7 ){ // avance3
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 ){ xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4, xAttA = xA, spriteA = 6 ; }
         if ( GOTOU == 0 && temps > reftemps + 27 ){ xA = xA + 1, spriteA = 0, spriteavanceA = 4, GOTOU = 1 ; }
    };

     if ( GOTOU == 0 && pA == 8 ){ //avance4;8
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && entrance == 0 ){ pA = 48, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 ){ xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4, xAttA = xA, spriteA = 0; }
         if ( GOTOU == 0 && temps > reftemps + 36 ){ pA = 0, reftemps = temps, spriteavanceA = 0, GOTOU = 1 ; }
    };


      //                                avance inversee;

       if ( GOTOU == 0 && pA == 9 ){ //avanceR
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){ xFA = xA, xTA = xA, xMA = xA, xGA = xA, xAttA = xA + 4 ; }
           if ( GOTOU == 0 ){ spriteA = 4, spriteavanceA = 1, xA = xA - 1, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 10 ){ //avance1R
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){ xFA = xA, xTA = xA, xMA = xA, xGA = xA, xAttA = xA + 4 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA - 1, spriteA = 5, spriteavanceA = 2, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 11 ){ // avance2R
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){ xFA = xA, xTA = xA, xMA = xA, xGA = xA, xAttA = xA + 4, spriteA = 5 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA - 1, spriteA = 6, spriteavanceA = 3, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 12 ){ //avance3R
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){ xFA = xA, xTA = xA, xMA = xA, xGA = xA, xAttA = xA + 4, spriteA = 6 ; }
           if ( GOTOU == 0 && temps > reftemps + 27 ){ xA = xA - 1, spriteA = 0, spriteavanceA = 4, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 13 ){ //avance4R
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && enterSorceror == 0 ){ pA = 49, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){ xFA = xA, xTA = xA, xMA = xA, xGA = xA, xAttA = xA + 4, spriteA = 0; }
           if ( GOTOU == 0 && temps > reftemps + 36 ){ pA = 1, reftemps = temps, spriteavanceA = 0, GOTOU = 1 ; }
      };


     //                               recule;
      if ( GOTOU == 0 && pA == 14 ){ //recule
         xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
         xAttA = xA;
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 ){ spriteA = 6, spritereculeA = 1, xA = xA - 1 ; }
         GOTOU = 1;
     };

     if ( GOTOU == 0 && pA == 15 ){ //recule1
        xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
        xAttA = xA;
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA - 1, spriteA = 5, spritereculeA = 2 ; }
        GOTOU = 1;
    };

     if ( GOTOU == 0 && pA == 16 ){ //recule2
        xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
        xAttA = xA;
        spriteA = 5;
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA - 1, spriteA = 4, spritereculeA = 3 ; }
        GOTOU = 1;
    };

     if ( GOTOU == 0 && pA == 17 ){ //recule3
        xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
        xAttA = xA;
        spriteA = 4;
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 && temps > reftemps + 27 ){ xA = xA - 1, spriteA = 0, spritereculeA = 4 ; }
        GOTOU = 1;
    };

     if ( GOTOU == 0 && pA == 18 ){ //recule4
        xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
        xAttA = xA;
        spriteA = 0;
         if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 58, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
         if ( GOTOU == 0 && temps > reftemps + 36 ){ pA = 0, reftemps = temps, spritereculeA = 0 ; }
        GOTOU = 1;
    };


      //                               recule inverse;

       if ( GOTOU == 0 && pA == 19 ){ //reculeR
          xAttA = xA + 4;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
          if ( GOTOU == 0 ){ spriteA = 6, spritereculeA = 1, xA = xA + 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 20 ){ //recule1R
          xAttA = xA + 4;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA + 1, spriteA = 5, spritereculeA = 2 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 21 ){ //recule2R
          xAttA = xA + 4;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          spriteA = 5;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA + 1, spriteA = 4, spritereculeA = 3 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 22 ){ //recule3R
          xAttA = xA + 4;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          spriteA = 4;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 27 ){ xA = xA + 1, spriteA = 0, spritereculeA = 4 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 23 ){ //recule4R
          xAttA = xA + 4;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          spriteA = 0;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 && sortieA == 0 ){ pA = 59, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 36 ){ pA = 1, reftemps = temps, spritereculeA = 0 ; }
          GOTOU = 1;
      };


      if ( GOTOU == 0 && pA == 24 ){ //saute
          xAttA = xA, xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          decapitated[0] = 0;
          yGA = yMA, yAttA = 14;
	   if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 46, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }

           if ( GOTOU == 0 && temps > reftemps + 51 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 50 ){ spriteA = 0, xMA = xA + 4, xGA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 7, xMA = xA + 4, xGA = xA + 1, decapitated[0] = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 8, xMA = xA + 1, xGA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 7, xMA = xA + 4, xGA = xA + 1, GOTOU = 1 ; }
      };

      if ( GOTOU == 0 && pA == 25 ){ //sauteR
         xAttA = xA + 4, xFA = xA, xTA = xA, xMA = xA, xGA = xA;
         decapitated[0] = 0;
         yGA = yMA, yAttA = 14;
          if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 47, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }

          if ( GOTOU == 0 && temps > reftemps + 51 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 50 ){ spriteA = 0, xMA = xA, xGA = xA, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 7, xMA = xA, xGA = xA + 1, decapitated[0] = 1, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 8, xMA = xA + 3, xGA = xA + 3, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 7, xMA = xA, xGA = xA + 3, GOTOU = 1 ; }
     };

       if ( GOTOU == 0 && pA == 26 ){ //assis
          xAttA = xA;
          xFA = xA, xTA = xA, xMA = xA + 4, xGA = xA + 4;
          spriteA = 9;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 50, Aoccupe = 1, reftemps = temps , GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ pA = 27, GOTOU = 1 ; }
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };

       if ( GOTOU == 0 && pA == 27 ){ //assis2
          Aoccupe = 0;
          assisA = 1;
          xAttA = xA;
          xFA = xA, xTA = xA, xMA = xA + 4, xGA = xA + 4;
          spriteA = 10;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 50, Aoccupe = 1, reftemps = temps , GOTOU = 26 ; }
           if ( GOTOU == 0 && demo == 1 ){
               if ( GOTOU == 0 && temps > reftemps + 10 ){ Aoccupe = 0 , GOTOU = 1 ; }
              GOTOU = 1;
          };
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };


       if ( GOTOU == 0 && pA == 28 ){ //releve
          xAttA = xA, yAttA = 14;
          xFA = xA, xTA = xA, xMA = xA + 4, xGA = xA + 4;
          spriteA = 9;
           if ( GOTOU == 0 && temps > reftemps + 5 ){ pA = 0, Aoccupe = 0, GOTOU = 1 ; }
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };

       if ( GOTOU == 0 && pA == 29 ){ //assisR
          xAttA = xA + 4;
          xFA = xA + 4, xTA = xA + 4, xMA = xA, xGA = xA + 4;
          spriteA = 9;
          if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 51, Aoccupe = 1, reftemps = temps , GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ pA = 30, GOTOU = 1 ; }
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };

       if ( GOTOU == 0 && pA == 30 ){ //assis2R
          Aoccupe = 0;
          assisA = 1;
          xAttA = xA + 4;
          xFA = xA + 4, xTA = xA + 4, xMA = xA, xGA = xA + 4;
          spriteA = 10;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 51, Aoccupe = 1, reftemps = temps , GOTOU = 26 ; }
           if ( GOTOU == 0 && demo == 1 ){
               if ( GOTOU == 0 && temps > reftemps + 10 ){ Aoccupe = 0 ; }
              GOTOU = 1;
          };
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };

       if ( GOTOU == 0 && pA == 31 ){ //releveR
          xAttA = xA + 4, yAttA = 14;
          xFA = xA + 4, xTA = xA + 4, xMA = xA, xGA = xA;
          spriteA = 9;
           if ( GOTOU == 0 && temps > reftemps + 5 ){ pA = 1, Aoccupe = 0, GOTOU = 1 ; }
          if ( GOTOU == 0 ){ GOTOU = 1; }
      };


       if ( GOTOU == 0 && pA == 32 ){ //rouladeAV
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20, yAttA = yGA, xAttA = xA, yTA = yGA;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ yTA = 16, pA = 54, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 49 ){
              spriteA = 0, xTA = xA + 4, xMA = xA + 4, yTA = 16 ;
              Aoccupe = 0, xA = xA + 1, pA = 33, GOTOU = 4;
          };
           if ( GOTOU == 0 && temps > reftemps + 45 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 42 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 41 ){ xA = xA + 1, spriteA = 16, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 38 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ xA = xA + 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 33 ){ xA = xA + 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 29 ){ xA = xA + 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA + 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA + 1, xAttA = xA + 5, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ xA = xA + 1, xAttA = xA + 5, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ xA = xA + 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA + 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA + 1, spriteA = 11, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ xA = xA + 1, spriteA = 11,  GOTOU = 1 , PlayAPCM(1, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 11, GOTOU = 1 ; }
      };

      //finderoulade ;
       if ( GOTOU == 4 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pA == 33 ){
           if ( GOTOU == 0 && xA < xB ){
              pA = 0;
              spriteA = 0;
              spriteavanceA = 0;
              xAttA = xA, yAttA = 17;
              yFA = 15, yTA = 16, yMA = 18, yGA = 20;
              xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
              GOTOU = 1;
          };
           if ( GOTOU == 0 && xA > xB - 1 ){
              pA = 62, yAttA = 14, Aoccupe = 1, reftemps = temps;
              pB = 62, yAttB = 14, Boccupe = 1, reftempsB = temps;
              GOTOU = 31;
          };
      };

       if ( GOTOU == 0 && pA == 34 ){ //rouladeAVR
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20, yAttA = yGA, xAttA = xA + 4, yTA = yGA;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 55, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 49 ){
              spriteA = 0, xTA = xA, xMA = xA ;
              Aoccupe = 0, xA = xA - 1, pA = 35, GOTOU = 5;
          };
           if ( GOTOU == 0 && temps > reftemps + 45 ){ yTA = 16, spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 42 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 41 ){ xA = xA - 1, spriteA = 16, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 38 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ xA = xA - 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 33 ){ xA = xA - 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 29 ){ xA = xA - 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA - 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA - 1, xAttA = xA - 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ xA = xA - 1, xAttA = xA - 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ xA = xA - 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA - 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA - 1, spriteA = 11, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ xA = xA - 1, spriteA = 11, GOTOU = 1 , PlayAPCM(1, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 11, GOTOU = 1 ; }
      };

      //finderouladeR ;
       if ( GOTOU == 5 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pA == 35 ){
           if ( GOTOU == 0 && xA > xB ){
              pA = 1;
              spriteA = 0;
              spriteavanceA = 0;
              xAttA = xA + 4, yAttA = 17;
              yFA = 15, yTA = 16, yMA = 18, yGA = 20;
              xFA = xA, xTA = xA, xMA = xA, xGA = xA;
              GOTOU = 1;
          };
           if ( GOTOU == 0 && xA < xB + 1 ){
              pA = 63, yAttA = 14, Aoccupe = 1, reftemps = temps;
              pB = 63, yAttB = 14, Boccupe = 1, reftempsB = temps;
              GOTOU = 31;
          };

      };


       if ( GOTOU == 0 && pA == 36 ){ //rouladeAR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20, yAttA = yGA, xAttA = xA;

           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 60, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 44 ){
              spriteA = 0, xTA = xA + 4, xMA = xA + 4 ;
              spritereculeA = 0;
              Aoccupe = 0, pA = 0, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 41 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 38 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ xA = xA - 1, spriteA = 11, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 33 ){ xA = xA - 1, spriteA = 11, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 29 ){ xA = xA - 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA - 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA - 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ xA = xA - 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ xA = xA - 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA - 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA - 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ xA = xA - 1, spriteA = 16, GOTOU = 1 , PlayAPCM(1, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 16, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 37 ){ //rouladeARR
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20, yAttA = yGA, xAttA = xA + 4;
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 61, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 && temps > reftemps + 44 ){
              spriteA = 0, xTA = xA, xMA = xA;
              spritereculeA = 0;
              Aoccupe = 0, pA = 1, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 41 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 38 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ xA = xA + 1, spriteA = 11, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 33 ){ xA = xA + 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 29 ){ xA = xA + 1, spriteA = 12, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA + 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA + 1, spriteA = 13, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ xA = xA + 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ xA = xA + 1, spriteA = 14, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ xA = xA + 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA + 1, spriteA = 15, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ xA = xA + 1, spriteA = 16,  GOTOU = 1 , PlayAPCM(1, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 16, GOTOU = 1 ; }
      };


          if ( GOTOU == 0 && pA == 38 ){ //protegeH1
              if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 52, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
	     if ( GOTOU == 0 ){
	             xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
        	     xAttA = xA;
	             yGA = 20;
		};
              if ( GOTOU == 0 && temps > reftemps + 3 ){ protegeHA = 1, spriteA = 17, Aoccupe = 0, GOTOU = 1 ; }
              if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
              if ( GOTOU == 0 && temps > reftemps + 1 ){  xA = xA - 1, spriteA = 4, GOTOU = 1 , PlayAPCM(1, protege, 0) ; }
              if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 4, GOTOU = 1 ; }
         };

        if ( GOTOU == 0 && pA == 39 ){ //protegeH
	     if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 52, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
	     if ( GOTOU == 0 ){
    		     xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
	             xAttA = xA;
        	     yGA = 20;
	             spriteA = 17;
	             GOTOU = 1;
		};
         };

       if ( GOTOU == 0 && pA == 40 ){ //protegeHR1
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 53, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
	     if ( GOTOU == 0 ){
          	xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          	xAttA = xA + 4;
          	yGA = 20;
	    };
           if ( GOTOU == 0 && temps > reftemps + 3 ){ protegeHA = 1, spriteA = 17, Aoccupe = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ xA = xA + 1, spriteA = 4, GOTOU = 1 , PlayAPCM(1, protege, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 4, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 41 ){ //protegeHR
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 53, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
	     if ( GOTOU == 0 ){
           	xFA = xA, xTA = xA, xMA = xA, xGA = xA;
           	xAttA = xA + 4;
           	yGA = 20;
           	spriteA = 17;
          	GOTOU = 1;
	    };
      };


       if ( GOTOU == 0 && pA == 42 ){ //protegeD1
              if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 56, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
              if ( GOTOU == 0 ){
		xAttA = xA;
                 yGA = 20;
                 xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
                 decapitated[0] = 0;
                 spriteA = 17;
	     };
              if ( GOTOU == 0 && temps > reftemps + 3 ){ pA = 43, protegeDA = 1, Aoccupe = 0, GOTOU = 1 ; }
              if ( GOTOU == 0 && temps > reftemps + 1 ){ GOTOU = 1 ; }
              if ( GOTOU == 0 && temps >= reftemps ){ PlayAPCM(1, protege, 0) ; }
      };

       if ( GOTOU == 0 && pA == 43 ){ //protegeD
		if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 56, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
		if ( GOTOU == 0 ){
			 xAttA = xA;
		         yGA = 20;
	             	 xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
	             	 decapitated[0] = 0;
	             	 spriteA = 18, GOTOU = 1;
		 ; }
      };

       if ( GOTOU == 0 && pA == 44 ){ //protegeDR1
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 57, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){
		xAttA = xA + 4;
	          yGA = 20;
        	  xFA = xA, xTA = xA, xMA = xA, xGA = xA;
	          decapitated[0] = 0;
        	  spriteA = 17;
	   ; }
           if ( GOTOU == 0 && temps > reftemps + 3 ){ spriteA = 17, protegeDA = 1, Aoccupe = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps >= reftemps ){ PlayAPCM(1, protege, 0) ; }
      };

       if ( GOTOU == 0 && pA == 45 ){ //protegeDR
           if ( GOTOU == 0 && attack[0] == 1 && demo == 0 ){ pA = 57, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
           if ( GOTOU == 0 ){
		   xAttA = xA + 4;
        	   yGA = 20;
	           xFA = xA, xTA = xA, xMA = xA, xGA = xA;
	           decapitated[0] = 0;
	           spriteA = 18;
		   GOTOU = 1;
	   ; }
      };

       if ( GOTOU == 0 && pA == 46 ){ //cou
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 19, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){
              spriteA = 20;
               if ( pB == 46 && distance < 12 ){
                  if ( spriteB == 19 || spriteB == 20 ){
			SwordCling();
                   GOTOU = 1;
				  };
              };
              xTA = xA, xAttA = xA + 7, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 19, yAttA = yTA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 17, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 47 ){ //couR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 19, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){
              spriteA = 20;
               if ( pB == 47 && distance < 12 ){
                  if ( spriteB == 19 || spriteB == 20 ){
			SwordCling();
                   GOTOU = 1;
				  };
              };
              xTA = xA + 4, xAttA = xA - 3, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 19, yAttA = yTA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 17, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 48 ){ // devant
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 22, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 23, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){
              spriteA = 23;
               if ( pB == 48 && distance < 9 ){
                  if ( spriteB == 22 || spriteB == 23 ){
			SwordCling();
                   GOTOU = 1;
				  };
              };
               if ( GOTOU == 0 && pB == 43 && distance < 7 ){
			SwordCling();
                  GOTOU = 1;
              };
              xMA = xA, xAttA = xA + 6, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 22, yAttA = yMA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 21, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 49 ){ // devantR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 22, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 23, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){
              spriteA = 23;
               if ( pB == 49 && distance < 9 ){
                  if ( spriteB == 22 || spriteB == 23 ){
			SwordCling();
                   GOTOU = 1;
				  };
              };
               if ( GOTOU == 0 && pB == 45 && distance < 7 ){
			SwordCling();
                  GOTOU = 1;
              };
              xMA = xA + 4, xAttA = xA - 2, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 22, yAttA = yMA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 21, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 50 ){ //genou
          xFA = xA, xTA = xA, xMA = xA, xGA = xA + 4;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 27, assisA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ spriteA = 10, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 25, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){
              spriteA = 25 ;
               if ( pB == 50 && distance <= 11 ){
                  if ( spriteB == 24 || spriteB == 25 ){
			SwordCling();
                   GOTOU = 1;
				  };
              };
              xGA = xA, xAttA = xA + 7, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 10, yAttA = yGA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 24, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 51 ){ //genouR
          yGA = 20;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ Aoccupe = 0, pA = 30, assisA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ spriteA = 10, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 25, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){
              spriteA = 25;
               if ( pB == 51 && distance <= 11 ){
                  if ( spriteB == 24 || spriteB == 25 ){
 			SwordCling();
                   GOTOU = 1;
				  };
              };
              xGA = xA + 4, xAttA = xA - 3, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 10, yAttA = yGA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){
                spriteA = 24;
                GOTOU = 1 ;
                }
      };

       if ( GOTOU == 0 && pA == 52 ){ // araignee
          xFA = xA + 4, xTA = xA + 4, xMA = xA, xGA = xA + 4;
          yAttA = yGA, xAttA = xA, yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 30 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){
              PlayAPCM(1, epee, 0);
              spriteA = 29;
              distance = xB - xA;
               if ( pB == 52 && distance <= 9 ){
               		SwordCling();
                  GOTOU = 1;
              };
              xAttA = xA + 5, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 15 ){ spriteA = 28, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 27, xAttA = xA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 26, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 53 ){ // araigneeR
          xFA = xA, xTA = xA, xMA = xA + 4, xGA = xA;
          yAttA = yGA, xAttA = xA + 4, yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 30 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){
              PlayAPCM(1, epee, 0);
              spriteA = 29;
              distance = xA - xB;
               if ( pB == 53 && distance <= 9 ){
			SwordCling();
                  GOTOU = 1;
              };
              xAttA = xA - 1, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 15 ){ spriteA = 28, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 27, xAttA = xA + 4, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 26, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 54 ){ //coupdepied
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          spriteA = 30, yAttA = yGA, yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 40 ){ Aoccupe = 0, spriteA = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 30, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ spriteA = 31, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ spriteA = 31, xAttA = xA + 5, GOTOU = 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 55 ){ //coupdepiedR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          spriteA = 30, yAttA = yGA, yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 40 ){ Aoccupe = 0, spriteA = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ spriteA = 30, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ spriteA = 31, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ spriteA = 31, xAttA = xA - 1, GOTOU = 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 56 ){ //coupdetete
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 30 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA - 1, spriteA = 32, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ spriteA = 33, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ xA = xA + 1, spriteA = 33, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ spriteA = 32, yAttA = yTA, GOTOU = 1 ; }
	   if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 32, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 57 ){ //coupdeteteR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20, yTA = 16;
           if ( GOTOU == 0 && temps > reftemps + 30 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 26 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ xA = xA + 1, spriteA = 32, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 11 ){ spriteA = 33, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ xA = xA - 1, spriteA = 33, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ spriteA = 32, yAttA = yTA, GOTOU = 1 ; }
	   if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 32, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 58 ){ //decapite
          decapitated[0] = 0;
          xFA = xA + 3, xTA = xA + 2, xMA = xA + 2, xGA = xA + 4;
           if ( GOTOU == 0 && temps > reftemps + 44 ){ Aoccupe = 0, spriteA = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 40 ){ spriteA = 19, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 35 ){ spriteA = 20, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ spriteA = 20, xTA = xA, xAttA = xA + 6, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 32 ){ spriteA = 20, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 19, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 27 ){ spriteA = 17, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 24 ){ xA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ xA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 12 ){ xA = xA + 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ spriteA = 37, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){ xA = xA + 1, spriteA = 37,  GOTOU = 1 , PlayAPCM(1, decapite, 0) ;  }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 37, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 59 ){ //decapiteR
          decapitated[0] = 0;
          xFA = xA + 1, xTA = xA + 2, xMA = xA + 2, xGA = xA;
           if ( GOTOU == 0 && temps > reftemps + 44 ){ Aoccupe = 0, spriteA = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 40 ){ spriteA = 19, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 35 ){ spriteA = 20, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 34 ){ spriteA = 20, xTA = xA + 4, xAttA = xA - 2, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 32 ){ spriteA = 20, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 19, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 27 ){ spriteA = 17, yAttA = yTA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 24 ){ xA = xA - 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 22 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 21 ){ xA = xA - 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ xA = xA - 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ xA = xA - 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 13 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 12 ){ xA = xA - 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ spriteA = 37, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){ xA = xA - 1, spriteA = 37,  GOTOU = 1 , PlayAPCM(1, decapite, 0) ;  }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 37, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 60 ){ //front
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 40 ){ Aoccupe = 0, pA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ spriteA = 35, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 36, xAttA = xA, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){
              spriteA = 36, distance = xB - xA;
               if ( pB == 60 && distance < 11 ){
                 if ( spriteB == 35 || spriteB == 36 ){
			SwordCling();
				 };
              };
               if ( pB == 39 && distance < 9 ){
			SwordCling();
              };
              xFA = xA, xAttA = xA + 7, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 9 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 35, yAttA = yFA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps ){ spriteA = 34, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 61 ){ //frontR
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yGA = 20;
           if ( GOTOU == 0 && temps > reftemps + 40 ){ Aoccupe = 0, pA = 1, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 25 ){ spriteA = 35, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 17 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 36, xAttA = xA + 4, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){
              spriteA = 36, distance = xA - xB;
              if ( pB == 61 && distance < 11 ){
                if ( spriteB == 35 || spriteB == 36 ){
			SwordCling();
				};
              };
               if ( pB == 41 && distance < 9 ){
			SwordCling();
              };
              xFA = xA + 4, xAttA = xA - 3, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 9 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){
              PlayAPCM(1, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteA = 35, yAttA = yFA, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps ){ spriteA = 34, GOTOU = 1 ; }
      };

      if ( GOTOU == 0 && pA == 62 ){ //retourne
         xAttA = xA, xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
         yAttA = 14;
         spriteA = 37;
          if ( GOTOU == 0 && temps > reftemps + 21 ){ pA = 1, Aoccupe = 0, sens = 9, spriteA = 0, spriteB = 0, couleurok = 0, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 14 ){ spriteA = 39, GOTOU = 1 ; }
          if ( GOTOU == 0 && temps > reftemps + 7 ){ spriteA = 38, GOTOU = 1 ; }
         GOTOU = 1;
     };

       if ( GOTOU == 0 && pA == 63 ){ //retourneR
          xAttA = xA + 4, xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yAttA = 14;
          spriteA = 37;
           if ( GOTOU == 0 && temps > reftemps + 21 ){ pA = 0, Aoccupe = 0, sens = 6, couleurok = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 7 ){ spriteA = 38, GOTOU = 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 64 ){ //vainqueur
          spriteA = 40;
          decapitated[0] = 1;
          blood[0] = 0;
		  spritereculeA = 0;
          xAttA = xA, yGA = 20, yAttA = 14;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          //psgvol 0,0;
           if ( GOTOU == 0 && temps > reftemps + 100 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 86 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ spriteA = 42, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ spriteA = 41, GOTOU = 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 65 ){ //vainqueurR
          spriteA = 40;
          decapitated[0] = 1;
          blood[0] = 0;
		  spritereculeA = 0;
          xAttA = xFA + 4, yGA = 20, yAttA = 14;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          //psgvol 0,0;
           if ( GOTOU == 0 && temps > reftemps + 100 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 86 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 37 ){ spriteA = 42, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ spriteA = 41, GOTOU = 1 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 66 ){ //vainqueurKO
          decapitated[0] = 1;
          blood[0] = 0;
		  spritereculeA = 0;
          xAttA = xA, yGA = 20 ;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;

           if ( GOTOU == 0 && temps > reftemps + 230 ){
              gnome = 1, reftemps = temps;
              xGNOME = 3;
              xSPRt = 270;
              GOTOU = 31;
          };

           if ( GOTOU == 0 && temps > reftemps + 205 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 195 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 140 ){ spriteA = 42, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 123 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 105 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 100 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 75 ){ spriteA = 30, spriteB = 50, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 71 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 70 ){ spriteA = 31, spriteB = 49, xB = xB + 2, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 55 ){ spriteA = 30, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 40 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 36 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 35 ){
              distance = xB - xA;
               if ( GOTOU == 0 && distance < 5 ){ spriteA = 6, xA = xA - 1 ; }
               if ( GOTOU == 0 && distance > 5 ){ spriteA = 6, xA = xA + 1 ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 23 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){ spriteA = 37 ; }
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 67 ){ //vainqueurKOR
          decapitated[0] = 1;
          blood[0] = 0;
		  spritereculeA = 0;
          xAttA = xFA + 4, yGA = 20;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;

           if ( GOTOU == 0 && temps > reftemps + 230 ){
              gnome = 1, reftemps = temps;
              xGNOME = 3;
              xSPRt = 270;
              GOTOU = 31;
          };

           if ( GOTOU == 0 && temps > reftemps + 205 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 195 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 140 ){ spriteA = 42, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 123 ){ spriteA = 41, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 105 ){ spriteA = 40, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 100 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 75 ){ spriteA = 30, spriteB = 50, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 71 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 70 ){ spriteA = 31, spriteB = 49, xB = xB - 2, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 55 ){ spriteA = 30, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 40 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 36 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 35 ){
              distance = xA - xB;
               if ( GOTOU == 0 && distance < 5 ){ spriteA = 6, xA = xA + 1 ; }
               if ( GOTOU == 0 && distance > 5 ){ spriteA = 6, xA = xA - 1 ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 0, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 23 ){ spriteA = 39, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ spriteA = 38, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){ spriteA = 37 ; }
          GOTOU = 1;
      };


      //                               ******degats******;
       if ( GOTOU == 0 && pA == 68 ){ //touche
          attenteA = 0;
          xAttA = xA;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
          yFA = 15, yTA = 16, yMA = 18, yGA = 20;
           if ( GOTOU == 0 && pB == 54 ){ pA = 72, GOTOU = 26 ; }

            if ( GOTOU == 0 && serpent[0] == 0 ){ serpent[0] = 1, refserpent[0] = temps ; }

           if ( GOTOU == 0 && pB == 58 && decapitated[0] == 1 ){
              pA = 80, Aoccupe = 1, reftemps = temps;
              xSPRt = xA + 3, score[1] = score[1] + 250;
              blood[0] = 0, serpent[0] = 0;
              GOTOU = 34;
          };

           if ( GOTOU == 0 ){ life[0] = life[0] - 1 ; }
           if ( GOTOU == 0 && life[0] <= 0 ){
              serpent[0] = 0, blood[0] = 1;
              pA = 78, Aoccupe = 1, reftemps = temps, blood[0] = 1, GOTOU = 34;
          };

           if ( GOTOU == 0 ){
              pA = 69, Aoccupe = 1, reftemps = temps, decapitated[0] = 1;
              GOTOU = 1;
          };
      };


       if ( GOTOU == 0 && pA == 69 ){ //touche1
          attenteA = 0;
          xAttA = xA;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
           if ( GOTOU == 0 && temps > reftemps + 20 ){
              spriteA = 0, Aoccupe = 0;
                blood[0] = 0;
              pA = 0, GOTOU = 1;
          };
            if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ xA = xA - 1, blood[0] = 0, spriteA = 43, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA - 2, spriteA = 44, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){
              xA = xA - 1, spriteA = 43;
              choixTOUCHE = choixTOUCHE + 1;
               if ( choixTOUCHE > 3 ){ choixTOUCHE = 1 ; }
               if ( choixTOUCHE == 1 ){ PlayAPCM(1, touche, 0) ; }
               if ( choixTOUCHE == 2 ){ PlayAPCM(1, touche2, 0) ; }
               if ( choixTOUCHE == 3 ){ PlayAPCM(1, touche3, 0) ; }
              GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 43, blood[0] = 1, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 70 ){ //toucheR
          attenteA = 0;
          xAttA = xA + 4;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
          yFA = 15, yTA = 16, yMA = 18, yGA = 20;
           if ( GOTOU == 0 && pB == 55 ){ pA = 74, GOTOU = 26 ; }

            if ( GOTOU == 0 && serpent[0] == 0 ){ serpent[0] = 1, refserpent[0] = temps ; }

           if ( GOTOU == 0 && pB == 59 && decapitated[0] == 1 ){
              pA = 81, Aoccupe = 1, reftemps = temps;
              xSPRt = xA + 3, score[1] = score[1] + 250;
              blood[0] = 0, serpent[0] = 0;
              GOTOU = 34;
          };

           if ( GOTOU == 0 ){ life[0] = life[0] - 1 ; }

           if ( GOTOU == 0 && life[0] <= 0 ){
              blood[0] = 1, serpent[0] = 0;
              pA = 79, Aoccupe = 1, reftemps = temps, blood[0] = 1, GOTOU = 34;
          };

           if ( GOTOU == 0 ){
              pA = 71, Aoccupe = 1, reftemps = temps, decapitated[0] = 1;
              GOTOU = 1;
          };
      };

       if ( GOTOU == 0 && pA == 71 ){ //touche1R
          attenteA = 0;
          xAttA = xA + 4;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
           if ( GOTOU == 0 && temps > reftemps + 20 ){
              spriteA = 0, Aoccupe = 0;
              blood[0] = 0;
              pA = 1, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 11 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ xA = xA + 1, blood[0] = 0, spriteA = 43, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 5 ){ xA = xA + 2, spriteA = 44, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){
              xA = xA + 1, spriteA = 43;
              choixTOUCHE = choixTOUCHE + 1;
               if ( choixTOUCHE > 3 ){ choixTOUCHE = 1 ; }
               if ( choixTOUCHE == 1 ){ PlayAPCM(1, touche, 0) ; }
               if ( choixTOUCHE == 2 ){ PlayAPCM(1, touche2, 0) ; }
               if ( choixTOUCHE == 3 ){ PlayAPCM(1, touche3, 0) ; }
              GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 43, blood[0] = 1, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 72 ){ //tombe
          //psgvol 0,0;
          xAttA = xA;
          attenteA = 0;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;

          blood[0] = 1;
            if ( GOTOU == 0 && serpent[0] == 0 ){ serpent[0] = 1, refserpent[0] = temps ; }
          life[0] = life[0] - 1, score[1] = score[1] + 100;

           if ( GOTOU == 0 && pB == 32 ){ life[0] = life[0] + 1, blood[0] = 0, serpent[0] = 0, score[1] = score[1] - 100 ; }

           if ( GOTOU == 0 && life[0] <= 0 ){
                blood[0] = 1, serpent[0] = 0;
              pA = 78, Aoccupe = 1, reftemps = temps, blood[0] = 1;
               if ( GOTOU == 0  ){
			if ( pB == 56 || pB == 54 ){ blood[0] = 0 ; }
		};
              GOTOU = 34;
          };

           if ( GOTOU == 0 ){
			if ( pB == 56 || pB == 54 ){ score[1] = score[1] + 150, blood[0] = 0 ; }
		};

           if ( GOTOU == 0 ){ pA = 73, Aoccupe = 1, reftemps = temps, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 73 ){ //tombe1
          xAttA = xA;
          attenteA = 0;
          xFA = xA, xTA = xA, xMA = xA, xGA = xA;
           if ( GOTOU == 0 && temps > reftemps + 25 ){
              spriteA = 0, xA = xA - 1, pA = 0, Aoccupe = 0, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 47, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ blood[0] = 0, xA = xA - 2, spriteA = 46, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){
              xA = xA - 2;
              PlayAPCM(1, tombe, 0) ;
               if ( pB == 54 ){ PlayAPCM(1, coupdepied, 0) ; }
               if ( pB == 56 ){ PlayAPCM(1, coupdetete, 0) ; }
              GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 45, GOTOU = 1 ; }
      };


       if ( GOTOU == 0 && pA == 74 ){ //tombeR
          //psgvol 0,0;
          xAttA = xA + 4;
          attenteA = 0;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;

          blood[0] = 1;
            if ( GOTOU == 0 && serpent[0] == 0 ){ serpent[0] = 1, refserpent[0] = temps ; }
          life[0] = life[0] - 1, score[1] = score[1] + 100;

           if ( GOTOU == 0 && pB == 34 ){ life[0] = life[0] + 1, blood[0] = 0, serpent[0] = 0, score[1] = score[1] - 100 ; }

           if ( GOTOU == 0 && life[0] <= 0 ){
                blood[0] = 1, serpent[0] = 0;
              pA = 79, Aoccupe = 1, reftemps = temps, blood[0] = 1;
               if ( GOTOU == 0  ){
				if ( pB == 57 || pB == 55 ){ blood[0] = 0 ; }
				};
              GOTOU = 34;
          };

           if ( GOTOU == 0  ){
				if ( pB == 57 || pB == 55 ){ score[1] = score[1] + 150, blood[0] = 0 ; }
			};

           if ( GOTOU == 0 ){ pA = 75, Aoccupe = 1, reftemps = temps, GOTOU = 1 ; }
      };

       if ( GOTOU == 0 && pA == 75 ){ //tombe1R
          xAttA = xA + 4;
          attenteA = 0;
          xFA = xA + 4, xTA = xA + 4, xMA = xA + 4, xGA = xA + 4;
           if ( GOTOU == 0 && temps > reftemps + 25 ){
              spriteA = 0, xA = xA + 1, pA = 1, Aoccupe = 0, GOTOU = 1;
          };
           if ( GOTOU == 0 && temps > reftemps + 16 ){ spriteA = 47, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 9 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 8 ){ blood[0] = 0, xA = xA + 2, spriteA = 46, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){
              xA = xA + 2;
              PlayAPCM(1, tombe, 0) ;
              if ( pB == 55 ){ PlayAPCM(1, coupdepied, 0) ; }
              if ( pB == 57 ){ PlayAPCM(1, coupdetete, 0) ; }
              GOTOU = 1;
          };
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 45, GOTOU = 1 ; }
      };



      //                               bruit des epees  et decapitation loupee;
       if ( GOTOU == 0 && sens == 6 ){

           if ( GOTOU == 0 && pA == 76 ){ //clingD

               if ( GOTOU == 0 && pB == 58 && decapitated[0] == 0 ){ pA = 68, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 && pB == 50 ){ pA = 68, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }

               if ( GOTOU == 0 ){ pA = 43, GOTOU = 1 ; }
          };

           if ( GOTOU == 0 && pA == 77 ){ //clingH
              pA = 39, GOTOU = 1;
          };

      };

       if ( GOTOU == 0 && sens == 9 ){

           if ( GOTOU == 0 && pA == 76 ){ //clingD
               if ( GOTOU == 0 && pB == 59 && decapitated[0] == 0 ){ pA = 70, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }
               if ( GOTOU == 0 && pB == 51 ){ pA = 70, Aoccupe = 1, reftemps = temps, GOTOU = 26 ; }

               if ( GOTOU == 0 ){ pA = 45, GOTOU = 1 ; }
          };

           if ( GOTOU == 0 && pA == 77 ){ //clingH
              pA = 41, GOTOU = 1;
          };

      };



     ///                                   *************************************;
     ///                                   **********Death Management*********;
     ///                                   *************************************;

     //mort ;
     ProcessDeath();
      if ( GOTOU == 34 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pA == 78 ){ //mort
            sens = 6;
           if ( GOTOU == 0 && temps > reftemps + 35 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ blood[0] = 0, spriteA = 48, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 2 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ spriteA = 9,  GOTOU = 1 , PlayAPCM(1, mortKO, 0) ; }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 9, pB = 66, Boccupe = 1, reftempsB = temps , GOTOU = 1; }

      };

       if ( GOTOU == 0 && pA == 79 ){ //mortR
            sens = 9;
           if ( GOTOU == 0 && temps > reftemps + 30 ){ blood[0] = 0; spriteA = 48;}
           if ( GOTOU == 0 && temps > reftemps + 1 ){ spriteA = 9; PlayAPCM(1, mortKO, 0); }
           if ( GOTOU == 0 && temps >= reftemps ){ spriteA = 9; pB = 67; Boccupe = 1; reftempsB = temps;}
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 80 ){ //mortdecap
           if ( GOTOU == 0 && temps > reftemps + 90 ){ spriteA = 54;}
           if ( GOTOU == 0 && temps > reftemps + 80 ){ spriteA = 53; }
           if ( GOTOU == 0 && temps > reftemps + 70 ){ bloodDecap[0] = 0; }
           if ( GOTOU == 0 && temps > reftemps + 60 ){ bloodDecap[0] = 6; }
           if ( GOTOU == 0 && temps > reftemps + 50 ){ bloodDecap[0] = 5; }
           if ( GOTOU == 0 && temps > reftemps + 40 ){ bloodDecap[0] = 4; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 52,; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ bloodDecap[0] = 0; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ bloodDecap[0] = 3; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ bloodDecap[0] = 2; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ bloodDecap[0] = 1; }
           if ( GOTOU == 0 && temps >= reftemps ){
              spriteA = 51, pB = 64, Boccupe = 1, reftempsB = temps , PlayAPCM(1, mortdecap, 0) ;
          };
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 81 ){ //mortdecapR
           if ( GOTOU == 0 && temps > reftemps + 90 ){ spriteA = 54; }
           if ( GOTOU == 0 && temps > reftemps + 80 ){ spriteA = 53; }
           if ( GOTOU == 0 && temps > reftemps + 70 ){ bloodDecap[0] = 0; }
           if ( GOTOU == 0 && temps > reftemps + 60 ){ bloodDecap[0] = 6; }
           if ( GOTOU == 0 && temps > reftemps + 50 ){ bloodDecap[0] = 5;}
           if ( GOTOU == 0 && temps > reftemps + 40 ){ bloodDecap[0] = 4; }
           if ( GOTOU == 0 && temps > reftemps + 30 ){ spriteA = 52,; }
           if ( GOTOU == 0 && temps > reftemps + 20 ){ bloodDecap[0] = 0; }
           if ( GOTOU == 0 && temps > reftemps + 14 ){ bloodDecap[0] = 3; }
           if ( GOTOU == 0 && temps > reftemps + 10 ){ bloodDecap[0] = 2; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ bloodDecap[0] = 1;}
           if ( GOTOU == 0 && temps >= reftemps ){
              spriteA = 51, pB = 65, Boccupe = 1, reftempsB = temps, PlayAPCM(1, mortdecap, 0) ;
          };
          GOTOU = 1;
      };

       if ( GOTOU == 0 && pA == 82 ){ //mortsorceror
           if ( temps > reftemps + 51 ){ pA = 84, GOTOU = 1 ; }
          // sorcerorfini;
           if ( GOTOU == 0 && temps > reftemps + 50 ){ blood[0] = 0, spriteA = 50, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 36 ){ GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 35 ){ blood[0] = 0, spriteA = 49, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 15 ){ blood[0] = 0, spriteA = 48, GOTOU = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 1 ){ spriteA = 9, GOTOU = 1 ; }
           if ( GOTOU == 0 ){ spriteA = 9, GOTOU = 1 ; }
      };



     ///                         **************************************;
     ///                         *********     p 2    ************;
     ///                         **************************************;
     //debut p 2;
      if ( GOTOU == 1 ){ GOTOU = 0 ; }


      if ( GOTOU == 0 ){
		if ( sorceror == 1 || marianna == 1 ){ blood[1] = 0, GOTOU = 24 ; }
	 ; }

      if ( GOTOU == 0 && sortieA == 1 ){
          if ( pB == 78 || pB == 80 || pB == 79 || pB == 81 ){ GOTOU = 2 ; }
     };

      if ( GOTOU == 0 && sortieB == 1 ){ GOTOU = 21 ; }


      //                           ************degats sur pB************;

       if ( GOTOU == 0 && sens == 6 ){
           if ( xB > xA ){
               if ( xAttA >= xFB && yAttA == yFB ){
                    positionSANG = yFB;
                   if ( pB == 39 ){ pB = 77, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 72, infoBdegatF = infoBdegatF + 1, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA >= xTB && yAttA == yTB ){
                   positionSANG = yTB;
                   if ( pA == 56 ){ pB = 72, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 68, score[0] = score[0] + 250, infoBdegatT = infoBdegatT + 1, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA >= xMB && yAttA == yMB ){
                    positionSANG = yMB;
                   if ( pB == 43 ){ pB = 76, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 68, score[0] = score[0] + 250, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA >= xGB && yAttA == yGB ){
                    positionSANG = yGB;
                   if ( pA == 52 ){ pB = 72, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pA == 32 ){ pB = 72, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pB == 43 ){ pB = 76, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pA == 54 ){ pB = 72, infoBdegatG = infoBdegatG + 1, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 68, score[0] = score[0] + 100, infoBdegatG = infoBdegatG + 1, GOTOU = 24 ; }
              };
          };
      };

       if ( GOTOU == 0 && sens == 9 ){
           if ( xB < xA ){
               if ( xAttA <= xFB && yAttA == yFB ){
                    positionSANG = yFB;
                   if ( pB == 41 ){ pB = 77, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 74, infoBdegatF = infoBdegatF + 1, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA <= xTB && yAttA == yTB ){
                    positionSANG = yTB;
                   if ( pA == 57 ){ pB = 74, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 70, score[0] = score[0] + 250, infoBdegatT = infoBdegatT + 1, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA <= xMB && yAttA == yMB ){
                    positionSANG = yMB;
                   if ( pB == 45 ){ pB = 76, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 70, score[0] = score[0] + 250, GOTOU = 24 ; }
              };

               if ( GOTOU == 0 && xAttA <= xGB && yAttA == yGB ){
                    positionSANG = yGB;
                   if ( pA == 53 ){ pB = 74, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pA == 34 ){ pB = 74, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pB == 45 ){ pB = 76, GOTOU = 24 ; }
                   if ( GOTOU == 0 && pA == 55 ){ pB = 74, infoBdegatG = infoBdegatG + 1, GOTOU = 24 ; }
                   if ( GOTOU == 0 ){ pB = 70, score[0] = score[0] + 100, infoBdegatG = infoBdegatG + 1, GOTOU = 24 ; }
              };
          };
      };


      if ( GOTOU == 0 && Boccupe == 1 ){ GOTOU = 24 ; } // important pour les animations
      if ( GOTOU == 0 ){ blood[1] = 0 ; }

     ///                       ********************************************;
     ///                       ******* evenements clavier pB *********;
     ///                       ********************************************;

     //clavierB ;
      if ( GOTOU == 21 ){ GOTOU = 0 ; }


      if ( GOTOU == 0 ){

         x2 = 7, y2 = 7;
         level[1] = 5; // neutre

          if ( partie == 2 ){
             //                       entrance DES TOUCHES;

            if (keys[SDLK_i])
                y2 = y2 - 1 ;

            if (keys[SDLK_k])
                y2 = y2 + 1 ;

            if (keys[SDLK_j])
                x2 = x2 - 1 ;

            if (keys[SDLK_l])
                x2 = x2 + 1 ;


             /// touche Attaque;

             attack[1] = 0;

            if (keys[SDLK_SPACE])
                {
                    attack[1] = 1;
                };


             //                       amplitude du pave virtuel;
              if ( y2 > 9 ){ y2 = 9 ; }
              if ( y2 < 5 ){ y2 = 5 ; }
              if ( x2 > 9 ){ x2 = 9 ; }
              if ( x2 < 5 ){ x2 = 5 ; }

             //                        position du "levier"  virtuel;

              if ( sens == 6 ){
                  if ( x2 <= 6 && y2 <= 6 ){ level[1] = 1 ; }
                 // hautG;
                 if ( x2 >= 8 && y2 <= 6 ){ level[1] = 3 ; }
                 // hautD;
                  if ( x2 <= 6 && y2 >= 8 ){ level[1] = 7 ; }
                 //basG;
                  if ( x2 >= 8 && y2 >= 8 ){ level[1] = 9 ; }
                 // basD;
                  if ( x2 <= 6 && y2 == 7 ){ level[1] = 4 ; }
                 // gauche;
                  if ( x2 >= 8 && y2 == 7 ){ level[1] = 6 ; }
                 // droite;
                  if ( x2 == 7 && y2 >= 8 ){ level[1] = 8 ; }
                 // bas;
                  if ( x2 == 7 && y2 <= 6 ){ level[1] = 2 ; }
                 // haut;
             };

              if ( sens == 9 ){
                  if ( x2 <= 6 && y2 <= 6 ){ level[1] = 1 ; }
                  if ( x2 >= 8 && y2 <= 6 ){ level[1] = 3 ; }
                  if ( x2 <= 6 && y2 >= 8 ){ level[1] = 7 ; }
                  if ( x2 >= 8 && y2 >= 8 ){ level[1] = 9 ; }
                  if ( x2 <= 6 && y2 == 7 ){ level[1] = 4 ; }
                  if ( x2 >= 8 && y2 == 7 ){ level[1] = 6 ; }
                  if ( x2 == 7 && y2 >= 8 ){ level[1] = 8 ; }
                  if ( x2 == 7 && y2 <= 6 ){ level[1] = 2 ; }
             };
         };
     };

     ///                       **********************************;
     ///                       ******* animations  auto *********;
     ///                       **********************************;

	if ( GOTOU == 0 && entrance == 1 ){
		level[1] = 4, GOTOU = 20;
	};


      if ( GOTOU == 0 && sortieB == 1 ){
          if ( GOTOU == 0 && sens == 9 ){ level[1] = 4, GOTOU = 20 ; }

          if ( GOTOU == 0 ){ sens = 6, level[1] = 6, GOTOU = 20 ; }
     };


      ///                       *****************************************;
      ///                       ******* Artificial Intelligence *******;
      ///                       *****************************************;


       if ( GOTOU == 0 && partie == 1 ){

          // ***************************IA de 1,2,3,6;
           if ( IA == 0 || IA == 1 || IA == 2 || IA == 3 || IA == 6 ){
               if ( sens == 6 ){
                  distance = xB - xA;

                   if ( temps <= 140 ){ /* bug shining */
                           if ( pA == 58 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                  };

                   if ( GOTOU == 0 && distance >= 15 ){ //  when too far
                      pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                  };

                   if ( GOTOU == 0 && IA == 6 ){
                       if ( distance < 15 ){
                           if ( pA == 58 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && IA == 3 ){
                       if ( distance < 15 ){
                           if ( infoBdegatT > 2 ){
                               if ( pA == 58 ){ pB = 27, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && pA == 58 ){ pB = 43, reftempsB = temps, GOTOU = 24 ; }
                      };
                  };


                   if ( GOTOU == 0 && distance == 12 && pA == 0 ){ pB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                   if ( GOTOU == 0 && distance > 9 && distance < 15 ){ // pour se rapprocher;
                       if ( level[0] == 4 ){ pB = 0, GOTOU = 24 ; }
                       if ( GOTOU == 0 ){ level[1] = 4, GOTOU = 20 ; }
                  };

                   if ( GOTOU == 0 && distance == 9 ){
                       if ( attenteA > 50 ){ level[1] = 4, GOTOU = 20 ; }
                       if ( GOTOU == 0 && pA == 36 ){ pB = 48, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && Aoccupe == 1 ){ level[1] = 4, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance < 9 && distance > 6 ){ // distance de combat 1;
                      // pour autoriser les croisements;
                       if ( demo == 0 && pA == 32 ){ pB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                      // pour se rapprocher;
                       if ( GOTOU == 0 && pA == 36 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && level[0] == 4 ){ pB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                      //pour eviter les degats repetitifs;

                       if ( GOTOU == 0 && IA > 1 ){
                           if ( infoBdegatG > 4 ){
                               if ( pA == 27 || pA == 50 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 27 || pA == 32 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ){
                               if ( pA == 46 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ){
                               if ( pA == 60 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                      };


                      //pour alterner les attaques;
                       if ( GOTOU == 0 && infocoupB == 0 ){ pB = 54, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 1 ){ pB = 0, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 2 ){ pB = 52, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 3 ){ pB = 0, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 4 ){ pB = 27, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 5 ){ pB = 50, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 6 ){ level[1] = 4, infocoupB = 0, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance <= 6 ){

                      // pour autoriser les croisements;
                       if ( demo == 0 && pA == 24 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && IA == 3 ){
                           if ( pA == 48 ){ pB = 43, reftempsB = temps, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA == 2 ){
                           if ( pA == 50 ){ pB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA > 1 ){
                           if ( infoBdegatG > 4 ){
                               if ( pA == 27 || pA == 50 ){ pB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 && IA > 2 ){
                                   if ( pA == 52 ){ pB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 54 ){ pB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ){
                                  if ( pA == 27 || pA == 50 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                      };

                       if ( GOTOU == 0 && infocoupB == 0 ){ pB = 54, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 1 ){ pB = 56, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 2 ){ pB = 52, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 3 ){ pB = 0, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 4 ){ pB = 27, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 5 ){ pB = 50, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 6 ){ pB = 0, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 7 ){ level[1] = 4, infocoupB = 0, GOTOU = 20 ; }

                  };
              };


               if ( GOTOU == 0 && sens == 9 ){

                  distance = xA - xB;

                   if ( GOTOU == 0 && distance >= 15 ){ //  quand trop loin;
                      pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                  };

                   if ( GOTOU == 0 && IA == 6 ){
                       if ( distance < 15 ){
                           if ( pA == 59 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && IA == 3 ){
                       if ( distance < 15 ){
                           if ( infoBdegatT > 2 ){
                               if ( pA == 59 ){ pB = 30, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && pA == 59 ){ pB = 45, reftempsB = temps, GOTOU = 24 ; }
                      };
                  };

                   if ( GOTOU == 0 && distance == 12 && pA == 1 ){ pB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                   if ( GOTOU == 0 && distance > 9 && distance < 15 ){ // pour se rapprocher;
                       if ( level[0] == 6 ){ pB = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 ){ level[1] = 6, GOTOU = 20 ; }
                  };

                   if ( GOTOU == 0 && distance == 9 ){
                       if ( attenteA > 50 ){ level[1] = 6, GOTOU = 20 ; }
                       if ( GOTOU == 0 && pA == 35 ){ pB = 49, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && Aoccupe == 1 ){ level[1] = 6, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance < 9 && distance > 6 ){ // distance de combat 1;
                      //authorize crossings
                       if ( demo == 0 && pA == 34 ){ pB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                      //get closer
                       if ( GOTOU == 0 && pA == 35 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && level[0] == 6 ){ pB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                      //avoid repetitive damage

                       if ( GOTOU == 0 && IA > 1 ){
                           if ( infoBdegatG > 4 ){
                               if ( pA ==9102 || pA == 51 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 30 || pA == 34 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ){
                               if ( pA == 47 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ){
                               if ( pA == 61 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                      };


                      //alternate attacks
                       if ( GOTOU == 0 && infocoupB == 0 ){ pB = 55, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 1 ){ pB = 1, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 2 ){ pB = 53, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 3 ){ pB = 1, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 4 ){ pB = 30, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 5 ){ pB = 51, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 6 ){ level[1] = 6, infocoupB = 0, GOTOU = 20 ; }

                  };

                   if ( GOTOU == 0 && distance <= 6 ){

                      // authorize crossings
                       if ( demo == 0 && pA == 25 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && IA == 3 ){
                           if ( pA == 49 ){ pB = 45, reftempsB = temps, GOTOU = 24 ; }
                      };

                       if ( GOTOU == 0 && IA == 2 ){
                           if ( pA == 51 ){ pB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                      };


                       if ( GOTOU == 0 && IA > 1 ){
                           if ( infoBdegatG > 4 ){
                               if ( pA == 30 || pA == 51 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 && IA > 2 ){
                                   if ( pA == 53 ){ pB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 55 ){ pB = 55, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0  ){
                                  if ( pA == 30 || pA == 55 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                      };

                       if ( GOTOU == 0 && infocoupB == 0 ){ pB = 55, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 1 ){ pB = 57, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 2 ){ pB = 53, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 3 ){ pB = 1, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 4 ){ pB = 30, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 5 ){ pB = 51, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 6 ){ pB = 1, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                       if ( GOTOU == 0 && infocoupB == 7 ){ level[1] = 6, infocoupB = 0, GOTOU = 20 ; }

                  };
              };

          };


          // ************************* IA de 4,5,6,7;

           if ( GOTOU == 0 ){
               if ( IA == 4 || IA == 5 || IA == 6 || IA == 7 ){

                   if ( sens == 6 ){
                      distance = xB - xA;

                       if ( GOTOU == 0 && distance >= 15 ){ //  quand trop loin;
                          pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                      };


                       if ( GOTOU == 0 && distance == 12 && pA == 0 ){ pB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && distance > 9 && distance < 15 ){ // pour se rapprocher;
                           if ( level[0] == 4 ){ pB = 0, GOTOU = 24 ; }
                           if ( GOTOU == 0 ){ level[1] = 4, GOTOU = 20 ; }
                      };

                       if ( GOTOU == 0 && distance == 9 ){
                           if ( attenteA > 50 ){ pB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && demo == 1 ){
                               if ( attenteA > 50 ){ pB = 58, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && pA == 36 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                           if ( GOTOU == 0 && IA == 7 ){
                               if ( Aoccupe == 1 ){ level[1] = 4, GOTOU = 20 ; }
                          };
                      };



                       if ( GOTOU == 0 && distance < 9 && distance > 6 ){ // distance de combat 1;
                          // pour autoriser les croisements;
                           if ( demo == 0 && pA == 32 ){ pB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                          // pour se rapprocher;
                           if ( GOTOU == 0 && pA == 36 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && level[0] == 4 ){ pB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                          //plus l IA est forte, plus il y des des coups imposes avant infocoupB ou infodegat;

                           if ( GOTOU == 0 && IA == 5 ){
                               if ( pA == 60 ){ pB = 39, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && distance < 15 && demo == 1 ){
                              pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                          };

                          //pour eviter les degats repetitifs;
                           if ( GOTOU == 0 && infoBdegatG > 4 ){
                               if ( pA == 27 || pA == 50 || pA == 52 ){ pB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 27 || pA == 50 || pA == 52 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ){
                               if ( pA == 46 ){ pB = 50, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ){
                               if ( IA < 7 ){
                                   if ( pA == 60 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };


                          //pour alterner les attaques;

                           if ( GOTOU == 0 && infocoupB == 0 ){ pB = 48, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 1 ){ pB = 60, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 2 ){ pB = 52, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 3 ){ pB = 52, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 4 ){ pB = 46, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 5 ){ level[1] = 4, infocoupB = 0, GOTOU = 20 ; }

                      };

                       if ( GOTOU == 0 && distance <= 6 ){

                          // pour autoriser les croisements;
                           if ( demo == 0 && pA == 24 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                           if ( GOTOU == 0 && IA > 4 ){
                               if ( pA == 48 ){ pB = 43, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && IA > 4 && IA < 7 ){
                               if ( pA == 50 ){ pB = 24, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatG > 4 ){
                               if ( pA == 27 || pA == 50 || pA == 52 ){ pB = 52, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 54 ){ pB = 54, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ){
                                   if ( pA == 27 || pA == 50 || pA == 52 ){ pB = 32, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };

                           if ( GOTOU == 0 && infocoupB == 0 ){ pB = 54, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 1 ){ pB = 56, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 2 ){ pB = 52, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 3 ){ pB = 50, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 4 ){ pB = 50, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 5 ){ level[1] = 4, infocoupB = 0, GOTOU = 20 ; }

                      };

                  };


                   if ( GOTOU == 0 && sens == 9 ){

                      distance = xA - xB;

                       if ( GOTOU == 0 && distance >= 15 ){ //  quand trop loin;
                          pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24;
                      };

                       if ( GOTOU == 0 && distance == 12 && pA == 1 ){ pB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                       if ( GOTOU == 0 && distance > 9 && distance < 15 ){ // pour se rapprocher;
                           if ( GOTOU == 0 && level[0] == 6 ){ pB = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 ){ level[1] = 6, GOTOU = 20 ; }
                      };

                       if ( GOTOU == 0 && distance == 9 ){
                           if ( GOTOU == 0 && attenteA > 50 ){ pB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && demo == 1 ){
                               if ( attenteA > 50 ){ pB = 59, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && pA == 35 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // if ( GOTOU == 0 && IA < 7 ){
                          // if ( GOTOU == 0 && Aoccupe == 1 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          // ; }
                           if ( GOTOU == 0 && IA == 7 ){
                               if ( Aoccupe == 1 ){ level[1] = 6, GOTOU = 20 ; }
                          };
                      };

                       if ( GOTOU == 0 && distance < 9 && distance > 6 ){ // distance de combat 1;
                          // pour autoriser les croisements;
                           if ( GOTOU == 0 && demo == 0 && pA == 32 ){ pB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }

                          // pour se rapprocher;
                           if ( GOTOU == 0 && pA == 35 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && level[0] == 6 ){ pB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                          //plus l IA est forte, plus il y des des coups imposes avant infocoupB ou infodegat;

                           if ( GOTOU == 0 && IA == 5 ){
                               if ( GOTOU == 0 && pA == 61 ){ pB = 41, reftempsB = temps, GOTOU = 24 ; }
                          };

                          //pour eviter les degats repetitifs;
                           if ( GOTOU == 0 && infoBdegatG > 4 ){
                               if ( pA == 30 || pA == 51 || pA == 53 ){ pB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( pA == 30 || pA == 51 || pA == 53 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatT > 2 ){
                               if ( GOTOU == 0 && pA == 47 ){ pB = 51, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatF > 2 ){
                               if ( GOTOU == 0 && IA < 7 ){
                                   if ( GOTOU == 0 && pA == 61 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };


                          //pour alterner les attaques;

                           if ( GOTOU == 0 && infocoupB == 0 ){ pB = 49, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 1 ){ pB = 61, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 2 ){ pB = 53, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 3 ){ pB = 53, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 4 ){ pB = 47, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 5 ){ level[1] = 6, infocoupB = 0, GOTOU = 20 ; }

                      };

                       if ( GOTOU == 0 && distance <= 6 ){

                          // pour autoriser les croisements;
                           if ( GOTOU == 0 && demo == 0 && pA == 25 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }


                           if ( GOTOU == 0 && IA > 4 ){
                               if ( GOTOU == 0 && pA == 49 ){ pB = 45, reftempsB = temps, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && IA > 4 && IA < 7 ){
                               if ( GOTOU == 0 && pA == 51 ){ pB = 25, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                          };

                           if ( GOTOU == 0 && infoBdegatG > 4 ){
                               if ( GOTOU == 0  ){
                                  if ( pA == 30 || pA == 51 || pA == 53 ){ pB = 53, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };
                           if ( GOTOU == 0 && infoBdegatG > 2 ){
                               if ( GOTOU == 0 && pA == 55 ){ pB = 55, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                               if ( GOTOU == 0 ){
                                   if ( pA == 30 || pA == 51 || pA == 53 ){ pB = 34, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                              };
                          };

                           if ( GOTOU == 0 && infocoupB == 0 ){ pB = 55, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 1 ){ pB = 57, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 2 ){ pB = 53, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 3 ){ pB = 51, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 4 ){ pB = 51, infocoupB = infocoupB + 1, reftempsB = temps, Boccupe = 1, GOTOU = 24 ; }
                           if ( GOTOU == 0 && infocoupB == 5 ){ level[1] = 6, infocoupB = 0, GOTOU = 20 ; }


                      };

                  };

              };
          };

      };




     //                       redirection suivant les touches;

      if ( GOTOU == 0 && level[1] == 1 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 3 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 7 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 9 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 8 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 2 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 4 ){ GOTOU = 20 ; }
      if ( GOTOU == 0 && level[1] == 6 ){ GOTOU = 20 ; }


     //                       actions si aucune touche n//a ete touchee;

      if ( GOTOU == 0 ){
         spriteavanceB = 0;
         spritereculeB = 0;

         protegeDB = 0, protegeHB = 0;

         attenteB = attenteB + 1;
         level[1] = 5;
     };

     // pour se relever;
      if ( GOTOU == 0 ){ assisB = 0 ; }
      if ( GOTOU == 0 && pB == 27 ){ pB = 28, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
      if ( GOTOU == 0 && pB == 30 ){ pB = 31, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }


     //attente des 5 secondes;
       if ( GOTOU == 0 && sens == 6 ){
          if ( attenteB > 300 ){ pB = 2, Boccupe = 1, reftempsB = temps, GOTOU = 24; }
     };
      if ( GOTOU == 0 && sens == 9 ){
          if ( attenteB > 300 ){ pB = 3, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
     };

      if ( GOTOU == 0 && sens == 6 ){ pB = 0 ; }
      if ( GOTOU == 0 && sens == 9 ){ pB = 1 ; }

     if ( GOTOU == 0 ){ GOTOU = 24 ; }


     ///                       *****************************************;
     ///                       *************actions p 2************;
     ///                       *****************************************;
     //actionB ;
      if ( GOTOU == 20 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 ){ attenteB = 1 ; }


     //                       droite,gauche, decapite, devant (normal);


      if ( GOTOU == 0 && sens == 6 ){
          if ( level[1] == 4 ){
             protegeDB = 0;
              if ( GOTOU == 0 && spriteavanceB == 1 ){ pB = 5, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 2 ){ pB = 6, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 3 ){ pB = 7, GOTOU = 24 ; }
              if ( GOTOU == 0 && spriteavanceB == 4 ){ pB = 8, GOTOU = 24 ; }
              if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	      if ( GOTOU == 0 ){ pB = 4, reftempsB = temps, GOTOU = 24 ; }
         };

          if ( GOTOU == 0 && level[1] == 6 ){
             protegeHB = 0;
              if ( GOTOU == 0 && spritereculeB == 1 ){ pB = 15, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 2 ){ pB = 16, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 3 ){ pB = 17, GOTOU = 24 ; }
              if ( GOTOU == 0 && spritereculeB == 4 ){ pB = 18, GOTOU = 24 ; }
              if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ){ pB = 14, reftempsB = temps, GOTOU = 24 ; }
         };
     };


      //                       droite, gauche, decapite, devant (inverse);

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 6 ){
              protegeDB = 0;
               if ( GOTOU == 0 && spriteavanceB == 1 ){ pB = 10, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 2 ){ pB = 11, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 3 ){ pB = 12, GOTOU = 24 ; }
               if ( GOTOU == 0 && spriteavanceB == 4 ){ pB = 13, GOTOU = 24 ; }
               if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 9, reftempsB = temps, GOTOU = 24 ; }
          };

           if ( GOTOU == 0 && level[1] == 4 ){
              protegeHB = 0;
               if ( GOTOU == 0 && spritereculeB == 1 ){ pB = 20, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 2 ){ pB = 21, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 3 ){ pB = 22, GOTOU = 24 ; }
               if ( GOTOU == 0 && spritereculeB == 4 ){ pB = 23, GOTOU = 24 ; }
               if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 19, reftempsB = temps, GOTOU = 24 ; }
          };
      };


     //                           saute, attaque cou;

      if ( GOTOU == 0 && sens == 6 ){
          if ( level[1] == 2 ){
             protegeDB = 0, protegeHB = 0;
             pB = 24, Boccupe = 1, reftempsB = temps, GOTOU = 24;
         };
     };

      if ( GOTOU == 0 && sens == 9 ){
          if ( level[1] == 2 ){
             protegeDB = 0, protegeHB = 0;
             pB = 25, Boccupe = 1, reftempsB = temps, GOTOU = 24;
         };
     };


      //                               assis, attaque genou;
       if ( GOTOU == 0 && sens == 6 ){
           if ( level[1] == 8 ){
               if ( attack[1] == 1 ){ pB = 50, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && assisB == 1 ){ pB = 27, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 26, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };
       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 8 ){
               if ( attack[1] == 1 ){ pB = 51, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && assisB == 1 ){ pB = 30, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 29, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                               roulade AV, coup de pied;
       if ( GOTOU == 0 && sens == 6 ){
           if ( level[1] == 7 ){
               if ( attack[1] == 1 ){ pB = 54, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ){ pB = 32, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 9 ){
               if ( attack[1] == 1 ){ pB = 55, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ){ pB = 34, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                               roulade AR, coup sur front;
       if ( GOTOU == 0 && sens == 6 ){
           if ( level[1] == 9 ){
               if ( attack[1] == 1 ){ pB = 60, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ){ pB = 36, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 7 ){
               if ( attack[1] == 1 ){ pB = 61, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
              if ( GOTOU == 0 ){ pB = 37, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

      //                                   protection Haute, araignee;
       if ( GOTOU == 0 && sens == 6 ){
           if ( level[1] == 3 ){
               if ( GOTOU == 0 && attack[1] == 1 ){ pB = 52, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeHB == 1 ){ pB = 39, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 38, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 1 ){
               if ( GOTOU == 0 && attack[1] == 1 ){ pB = 53, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeHB == 1 ){ pB = 41, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 40, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


      //                                   protection devant, coup de tete;
       if ( GOTOU == 0 && sens == 6 ){
           if ( level[1] == 1 ){
               if ( GOTOU == 0 && attack[1] == 1 ){ pB = 56, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && protegeDB == 1 ){ pB = 43, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 42, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };

       if ( GOTOU == 0 && sens == 9 ){
           if ( level[1] == 3 ){
               if ( GOTOU == 0 && attack[1] == 1 ){ pB = 57, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( protegeDB == 1 ){ pB = 45, GOTOU = 24 ; }
               if ( GOTOU == 0 ){ pB = 44, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          };
      };


     ///                       ***********************************;
     ///                       *********gestion p 2**********;
     ///                       ***********************************;


     //gestionB ;
      if ( GOTOU == 24 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 && pB == 0 ){ //debout
         spriteB = 0;
         // if ( GOTOU == 0 && marianna == 1 ){ spriteB = 57 ; }
         decapitated[1] = 1;
         blood[1] = 0;
         xAttB = xB + 4, yAttB = 14 ;
         yFB = 15, yTB = 16, yMB = 18, yGB = 20;
         xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          if ( GOTOU == 0 && partie == 1 ){
              if ( temps > reftempsB + 10 ){ Boccupe = 0 ; }
         };
         GOTOU = 2;
     };

      if ( GOTOU == 0 && pB == 1 ){ // deboutR
         spriteB = 0;
         decapitated[1] = 1;
         blood[1] = 0;
         xAttB = xB, yAttB = 14;
         yFB = 15, yTB = 16, yMB = 18, yGB = 20;
         xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          if ( GOTOU == 0 && partie == 1 ){
              if ( temps > reftempsB + 10 ){ Boccupe = 0 ; }
         };
         GOTOU = 2;
    };

      if ( GOTOU == 0 && pB == 2 ){ // attente
         xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          if ( GOTOU == 0 && temps > reftempsB + 100 ){ Boccupe = 0, attenteB = 1, pB = 0, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 74 ){ spriteB = 1, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 60 ){ spriteB = 2, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 46 ){ spriteB = 3, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 2, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 20 ){ GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 19 ){ spriteB = 1, GOTOU = 2 , PlayAPCM(2, attente, 0); }
         GOTOU = 2;
     };

      if ( GOTOU == 0 && pB == 3 ){ // attenteR
         xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          if ( GOTOU == 0 && temps > reftempsB + 100 ){ Boccupe = 0, attenteB = 1, pB = 1, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 74 ){ spriteB = 1, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 60 ){ spriteB = 2, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 46 ){ spriteB = 3, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 2, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 20 ){ GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 19 ){ spriteB = 1, GOTOU = 2 , PlayAPCM(2, attente, 0); }
         GOTOU = 2;
     };

     //                                   avance
      if ( GOTOU == 0 && pB == 4 ){
	 if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 ){ xFB = xB, xTB = xB, xMB = xB, xGB = xB, xAttB = xB + 4 ; }
         if ( GOTOU == 0 ){ spriteB = 4, spriteavanceB = 1, xB = xB - 1, GOTOU = 2 ; }
     };

     if ( GOTOU == 0 && pB == 5 ){//avance1
        if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
        if ( GOTOU == 0 ){ xFB = xB, xTB = xB, xMB = xB, xGB = xB, xAttB = xB + 4 ; }
        if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB - 1, spriteB = 5, spriteavanceB = 2, GOTOU = 2 ; }
    };

     if ( GOTOU == 0 && pB == 6 ){//avance2
        if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
        if ( GOTOU == 0 ){ xFB = xB, xTB = xB, xMB = xB, xGB = xB, xAttB = xB + 4, spriteB = 5 ; }
        if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB - 1, spriteB = 6, spriteavanceB = 3, GOTOU = 2 ; }
    };

     if ( GOTOU == 0 && pB == 7 ){//avance3
         if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 ){ xFB = xB, xTB = xB, xMB = xB, xGB = xB, xAttB = xB + 4, spriteB = 6 ; }
         if ( GOTOU == 0 && temps > reftempsB + 27 ){ xB = xB - 1, spriteB = 0, spriteavanceB = 4, GOTOU = 2 ; }
    };

     if ( GOTOU == 0 && pB == 8 ){//avance4
        if ( GOTOU == 0 && attack[1] == 1 && entrance == 0 ){ pB = 48, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
        if ( GOTOU == 0 ){ xFB = xB, xTB = xB, xMB = xB, xGB = xB, xAttB = xB + 4, spriteB = 0 ; }
        if ( GOTOU == 0 && temps > reftempsB + 36 ){ pB = 0, reftempsB = temps, spriteavanceB = 0, GOTOU = 2 ; }
    };



       if ( GOTOU == 0 && pB == 9 ){ // avanceR
          if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          if ( GOTOU == 0 ){ xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4, xAttB = xB ; }
          if ( GOTOU == 0 ){ spriteB = 4, spriteavanceB = 1, xB = xB + 1, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 10 ){//avance1R
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 ){ xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4, xAttB = xB ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB + 1, spriteB = 5, spriteavanceB = 2, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 11 ){//avance2R
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 ){ xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4, xAttB = xB, spriteB = 5 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB + 1, spriteB = 6, spriteavanceB = 3, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 12 ){//avance3R
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 ){ xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4, xAttB = xB, spriteB = 6 ; }
           if ( GOTOU == 0 && temps > reftempsB + 27 ){ xB = xB + 1, spriteB = 0, spriteavanceB = 4, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 13 ){//avance4R
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 49, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 ){ xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4, xAttB = xB, spriteB = 0 ; }
           if ( GOTOU == 0 && temps > reftempsB + 36 ){ pB = 1, reftempsB = temps, spriteavanceB = 0, GOTOU = 2 ; }
      };


     //                                        recule
      if ( GOTOU == 0 && pB == 14 ){
         xFB = xB, xTB = xB, xMB = xB, xGB = xB;
         xAttB = xB + 4;
          if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 ){ spriteB = 6, spritereculeB = 1, xB = xB + 1 ; }
         GOTOU = 2;
     };

     if ( GOTOU == 0 && pB == 15 ){//recule1
        xFB = xB, xTB = xB, xMB = xB, xGB = xB;
        xAttB = xB + 4;
         if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB + 1, spriteB = 5, spritereculeB = 2 ; }
        GOTOU = 2;
    };
     if ( GOTOU == 0 && pB == 16 ){//recule2
        xFB = xB, xTB = xB, xMB = xB, xGB = xB;
        xAttB = xB + 4;
        spriteB = 5;
         if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB + 1, spriteB = 4, spritereculeB = 3 ; }
        GOTOU = 2;
    };
     if ( GOTOU == 0 && pB == 17 ){//recule3
        xFB = xB, xTB = xB, xMB = xB, xGB = xB;
        xAttB = xB + 4;
        spriteB = 4;
         if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 && temps > reftempsB + 27 ){ xB = xB + 1, spriteB = 0, spritereculeB = 4 ; }
        GOTOU = 2;
    };
     if ( GOTOU == 0 && pB == 18 ){//recule4
        xFB = xB, xTB = xB, xMB = xB, xGB = xB;
        xAttB = xB + 4;
        spriteB = 0;
         if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 58, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
         if ( GOTOU == 0 && temps > reftempsB + 36 ){ pB = 0, reftempsB = temps, spritereculeB = 0 ; }
        GOTOU = 2;
    };


      //                               recule inverse
       if ( GOTOU == 0 && pB == 19 ){
          xAttB = xB;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
           if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          if ( GOTOU == 0 ){ spriteB = 6, spritereculeB = 1, xB = xB - 1 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 20 ){//recule1R
          xAttB = xB;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
           if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB - 1, spriteB = 5, spritereculeB = 2 ; }
          GOTOU = 2;
      };
       if ( GOTOU == 0 && pB == 21 ){//recule2R
          xAttB = xB;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          spriteB = 5;
           if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB - 1, spriteB = 4, spritereculeB = 3 ; }
          GOTOU = 2;
      };
       if ( GOTOU == 0 && pB == 22 ){//recule3R
          xAttB = xB;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          spriteB = 4;
           if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 27 ){ xB = xB - 1, spriteB = 0, spritereculeB = 4 ; }
          GOTOU = 2;
      };
       if ( GOTOU == 0 && pB == 23 ){//recule4R
          xAttB = xB;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          spriteB = 0;
           if ( GOTOU == 0 && attack[1] == 1 && sortieB == 0 ){ pB = 59, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 36 ){ pB = 1, reftempsB = temps, spritereculeB = 0 ; }
          GOTOU = 2;
      };


      if ( GOTOU == 0 && pB == 24 ){ // saute
        xAttB = xB + 4, xFB = xB, xTB = xB, xMB = xB, xGB = xB;
         yGB = yMB, yAttB = 14;
         decapitated[1] = 0;
          if ( GOTOU == 0 && attack[1] == 1 ){ pB = 46, Boccupe = 1, reftempsB = temps, GOTOU = 2 ; }

          if ( GOTOU == 0 && temps > reftempsB + 51 ){ Boccupe = 0, spriteB = 0, pB = 0, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 50 ){ spriteB = 0, xMB = xB, xGB = xB, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 7, xMB = xB, xGB = xB + 1, decapitated[1] = 1, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 8, xMB = xB + 3, xGB = xB + 3, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 1 ){ spriteB = 7, xMB = xB, xGB = xB + 3, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 7, GOTOU = 2 ; }
    };

       if ( GOTOU == 0 && pB == 25 ){ //sauteR
          xAttB = xB, xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yGB = yMB, yAttB = 14;
          decapitated[1] = 0;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 47, Boccupe = 1, reftempsB = temps, GOTOU = 2 ; }

           if ( GOTOU == 0 && temps > reftempsB + 51 ){ Boccupe = 0, spriteB = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 50 ){ spriteB = 0, xMB = xB + 4, xGB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 7, xMB = xB + 4, xGB = xB + 1, decapitated[1] = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 8, xMB = xB + 1, xGB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ spriteB = 7, xMB = xB + 4, xGB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 7, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 26 ){ // assis
          xAttB = xB + 4;
          xFB = xB + 4, xTB = xB + 4, xMB = xB, xGB = xB;
          spriteB = 9;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 50, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ pB = 27, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 27 ){ // assis2
          assisB = 1;
          xAttB = xB + 4;
          xFB = xB + 4, xTB = xB + 4, xMB = xB, xGB = xB;
          spriteB = 10;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 50, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && partie == 1 ){
               if ( GOTOU == 0 && temps > reftempsB + 10 ){ Boccupe = 0 ; }
              GOTOU = 2;
          };
          if ( GOTOU == 0 ){ Boccupe = 0, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 28 ){ //releve
          xAttB = xB + 4, yAttA = 14;
          xFB = xB + 4, xTB = xB + 4, xMB = xB, xGB = xB;
          spriteB = 9;
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ pB = 0, Boccupe = 0, GOTOU = 2 ; }
          GOTOU = 2;
      };


       if ( GOTOU == 0 && pB == 29 ){ // assisR
          xAttB = xB;
          xFB = xB, xTB = xB, xMB = xB + 4, xGB = xB + 4;
          spriteB = 9;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 51, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ pB = 30, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 30 ){ // assis2R
          assisB = 1;
          xAttB = xB;
          xFB = xB, xTB = xB, xMB = xB + 4, xGB = xB + 4;
          spriteB = 10;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 51, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && partie == 1 ){
               if ( GOTOU == 0 && temps > reftempsB + 10 ){ Boccupe = 0 ; }
              GOTOU = 2;
          };
          if ( GOTOU == 0 ){ Boccupe = 0, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 31 ){ // releveR
          xAttB = xB, yAttA = 14;
          xFB = xB, xTB = xB, xMB = xB + 4, xGB = xB + 4;
          spriteB = 9;
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ pB = 1, Boccupe = 0, GOTOU = 2 ; }
          GOTOU = 2;
      };


       if ( GOTOU == 0 && pB == 32 ){ // rouladeAV
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB;
          yGB = 20, yAttB = yGB, xAttB = xB + 4;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 54, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 49 ){
              spriteB = 0, xTB = xB, xMB = xB ;
              Boccupe = 0, xB = xB - 1, pB = 33, GOTOU = 6;
          };
           if ( GOTOU == 0 && temps > reftempsB + 45 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 42 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 41 ){ xB = xB - 1, spriteB = 16, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 38 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ xB = xB - 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 33 ){ xB = xB - 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 29 ){ xB = xB - 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB - 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB - 1, xAttB = xB - 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ xB = xB - 1, xAttB = xB - 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ xB = xB - 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB - 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB - 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB - 1, spriteB = 11, GOTOU = 2 , PlayAPCM(2, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 11, GOTOU = 2 ; }
      };

      ///finderouladeB ;
       if ( GOTOU == 6 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pB == 33 ){
           if ( GOTOU == 0 && xB > xA ){
              pB = 0;
              spriteB = 0;
              spriteavanceB = 0;
              xAttB = xB + 4, yAttB = 17;
              yFB = 15, yTB = 16, yMB = 18, yGB = 20;
              xFB = xB, xTB = xB, xMB = xB, xGB = xB;
              GOTOU = 2;
          };
           if ( GOTOU == 0 && xB < xA + 1 ){
              pA = 62, yAttA = 14, Aoccupe = 1, reftemps = temps;
              pB = 62, yAttB = 14, Boccupe = 1, reftempsB = temps;
              GOTOU = 31;
          };
      };

       if ( GOTOU == 0 && pB == 34 ){ //rouladeAVR
          xFB = xB, xTB = xB, xMB = xB, xGB = xB + 4;
          yGB = 20, yAttB = yGB, xAttB = xB;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 55, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 49 ){
              spriteB = 0, xTB = xB + 4, xMB = xB + 4 ;
              Boccupe = 0, xB = xB + 1, pB = 35, GOTOU = 7;
          };
           if ( GOTOU == 0 && temps > reftempsB + 45 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 42 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 41 ){ xB = xB + 1, spriteB = 16, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 38 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ xB = xB + 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 33 ){ xB = xB + 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 29 ){ xB = xB + 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB + 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB + 1, xAttB = xB + 5, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ xB = xB + 1, xAttB = xB + 5, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ xB = xB + 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB + 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB + 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB + 1, spriteB = 11, GOTOU = 2 , PlayAPCM(2, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 11, GOTOU = 2 ; }
      };

      //finderouladeBR ;
       if ( GOTOU == 7 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pB == 35 ){
           if ( GOTOU == 0 && xB < xA ){
              pB = 1;
              spriteB = 0;
              spriteavanceB = 0;
              xAttB = xB, yAttB = 17;
              yFB = 15, yTB = 16, yMB = 18, yGB = 20;
              xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
              GOTOU = 2;
          };
           if ( GOTOU == 0 && xB > xA - 1 ){
              pA = 63, Aoccupe = 1, reftemps = temps;
              pB = 63, Boccupe = 1, reftempsB = temps;
              GOTOU = 31;
          };
      };

       if ( GOTOU == 0 && pB == 36 ){ //rouladeAR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB;
          yGB = 20, yAttB = yGB, xAttB = xB + 4;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 60, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 44 ){
              spriteB = 0, xTB = xB, xMB = xB ;
              spritereculeB = 0;
              Boccupe = 0, pB = 0, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 41 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 38 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ xB = xB + 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 33 ){ xB = xB + 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 29 ){ xB = xB + 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB + 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB + 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ xB = xB + 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ xB = xB + 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB + 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB + 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB + 1, spriteB = 16, GOTOU = 2 , PlayAPCM(2, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 16, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 37 ){ //rouladeARR
          xFB = xB, xTB = xB, xMB = xB, xGB = xB + 4;
          yGB = 20, yAttB = yGB, xAttB = xB;
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 61, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
           if ( GOTOU == 0 && temps > reftempsB + 44 ){
              spriteB = 0, xTB = xB + 4, xMB = xB + 4 ;
              spritereculeB = 0;
              Boccupe = 0, pB = 1, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 41 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 38 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ xB = xB - 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 33 ){ xB = xB - 1, spriteB = 11, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 29 ){ xB = xB - 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB - 1, spriteB = 12, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB - 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ xB = xB - 1, spriteB = 13, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ xB = xB - 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ xB = xB - 1, spriteB = 14, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB - 1, spriteB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB - 1, spriteB = 16,  GOTOU = 2 , PlayAPCM(2, roule, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 16, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 38 ){ //protegeH1
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 52, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	   if ( GOTOU == 0 ){
	          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
	          xAttB = xB + 4;
	          yGB = 20;
	   ; }
           if ( GOTOU == 0 && temps > reftempsB + 3 ){ protegeHB = 1, pB = 39, Boccupe = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB + 1, spriteB = 4, GOTOU = 2 , PlayAPCM(2, protege, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 4, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 39 ){ //protegeH
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 52, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	   if ( GOTOU == 0 ){
	          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
	          xAttB = xB + 4;
	          yGB = 20;
	          spriteB = 17;
	          GOTOU = 2;
	   ; }
      };

       if ( GOTOU == 0 && pB == 40 ){ //protegeH1R
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 53, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	   if ( GOTOU == 0 ){
	          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
	          xAttB = xB;
	          yGB = 20;
	   ; }
           if ( GOTOU == 0 && temps > reftempsB + 3 ){ protegeHB = 1, pB = 41, Boccupe = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ xB = xB - 1, spriteB = 4, GOTOU = 2 , PlayAPCM(2, protege, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 4, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 41 ){ //protegeHR
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 53, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	   if ( GOTOU == 0 ){
	          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
	          xAttB = xB;
	          yGB = 20;
	          spriteB = 17;
	          GOTOU = 2;
	   ; }
      };

       if ( GOTOU == 0 && pB == 42 ){ // protegeD1
          if ( GOTOU == 0 && attack[1] == 1 ){ pB = 56, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
	  if ( GOTOU == 0 ){
		  xAttB = xB + 4;
	          yGB = 20;
	          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
	          decapitated[1] = 0;
	          spriteB = 17;
         };
           if ( GOTOU == 0 && temps > reftempsB + 3 ){ pB = 43, protegeDB = 1, Boccupe = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ PlayAPCM(2, protege, 0) ; }
      };

       if ( GOTOU == 0 && pB == 43 ){ //protedeD
          if ( GOTOU == 0 && attack[1] == 1 ){ pB = 56, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          if ( GOTOU == 0 ){
		  xAttB = xB + 4;
	          yGB = 20;
	          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
	          decapitated[1] = 0;
	          spriteB = 18;
		  GOTOU = 2;
	  ; }

      };

       if ( GOTOU == 0 && pB == 44 ){ //protegeD1R
          if ( GOTOU == 0 && attack[1] == 1 ){ pB = 57, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          if ( GOTOU == 0 ){
		  xAttB = xB;
	          yGB = 20;
	          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
	          decapitated[1] = 0;
	          spriteB = 17;
	   ; }
           if ( GOTOU == 0 && temps > reftempsB + 3 ){ pB = 45, protegeDB = 1, Boccupe = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ PlayAPCM(2, protege, 0) ; }
      };

       if ( GOTOU == 0 && pB == 45 ){ //protegeDR
           if ( GOTOU == 0 && attack[1] == 1 ){ pB = 57, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
          if ( GOTOU == 0 ){
		xAttB = xB;
	           yGB = 20;
	           xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
	           decapitated[1] = 0;
	           spriteB = 18;
	           GOTOU = 2;
	   ; }
      };


       if ( GOTOU == 0 && pB == 46 ){ // cou
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 19, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){
              spriteB = 20;
               if ( GOTOU == 0 && pA == 46 ){ GOTOU = 2 ; }
              xTB = xB + 4, xAttB = xB - 3, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 19, yAttB = yTB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 17, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 47 ){ // couR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 19, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){
              spriteB = 20;
               if ( GOTOU == 0 && pA == 47 ){ GOTOU = 2 ; }
              xTB = xB, xAttB = xB + 7, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 19, yAttB = yTB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 17, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 48 ){ // devant
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 22, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 23, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){
              spriteB = 23;
               if ( pA == 48 ){ GOTOU = 2 ; }
               if ( GOTOU == 0 && pA == 43 && distance < 7 ){
			SwordCling();
              };
              xMB = xB + 4, xAttB = xB - 2, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 22, yAttB = yMB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 21, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 49 ){ // devantR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 22, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 23, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){
              spriteB = 23;
               if ( pA == 49 ){ GOTOU = 2 ; }
               if ( GOTOU == 0 && pA == 45 && distance < 7 ){
			SwordCling();
              };
              xMB = xB, xAttB = xB + 6, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 22, yAttB = yMB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 21, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 50 ){ //genou
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 27, assisB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ spriteB = 10, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 25, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){
              spriteB = 25;
               if ( GOTOU == 0 && pA == 50 ){ GOTOU = 2 ; }
              xGB = xB + 4, xAttB = xB - 3, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 10, yAttB = yGB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 24, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 51 ){ // genouR
          xFB = xB, xTB = xB, xMB = xB, xGB = xB + 4;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ Boccupe = 0, pB = 30, assisB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ spriteB = 10, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 25, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){
              spriteB = 25;
               if ( GOTOU == 0 && pA == 51 ){ GOTOU = 2 ; }
              xGB = xB, xAttB = xB + 7, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 10, yAttB = yGB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 24, GOTOU = 2 ; }
      };


       if ( GOTOU == 0 && pB == 52 ){ // araignee
          xFB = xB, xTB = xB, xMB = xB + 4, xGB = xB;
          yAttB = yGB, xAttB = xB + 4, yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ Boccupe = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){
              PlayAPCM(2, epee, 0);
              spriteB = 29;
              xAttB = xB - 1, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ spriteB = 28, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 27, xAttB = xB + 4, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 26, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 53 ){ // araigneeR
          xFB = xB + 4, xTB = xB + 4, xMB = xB, xGB = xB + 4;
          yAttB = yGB, xAttB = xB, yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ Boccupe = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){
              PlayAPCM(2, epee, 0);
              spriteB = 29;
              xAttB = xB + 5, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ spriteB = 28, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 27, xAttB = xB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 26, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 54 ){ //coupdepied
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          spriteB = 30, yAttB = yGB, yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ Boccupe = 0, spriteB = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 30, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ spriteB = 31, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ spriteB = 31, xAttB = xB - 1, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 55 ){ //coupdepiedR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          spriteB = 30, yAttB = yGB, yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ Boccupe = 0, spriteB = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ spriteB = 30, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ spriteB = 31, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ spriteB = 31, xAttB = xB + 5, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 56 ){ //coupdetete
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ Boccupe = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB + 1, spriteB = 32, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ spriteB = 33, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ xB = xB - 1, spriteB = 33, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ spriteB = 32, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 32, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 57 ){ // coupdeteteR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ Boccupe = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ xB = xB - 1, spriteB = 32, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ spriteB = 33, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ xB = xB + 1, spriteB = 33, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ spriteB = 32, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 32, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 58 ){ //decapite
          decapitated[1] = 0;
          xFB = xB + 1, xTB = xB + 2, xMB = xB + 2, xGB = xB;
           if ( GOTOU == 0 && temps > reftempsB + 44 ){ Boccupe = 0, spriteB = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ spriteB = 19, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ spriteB = 20, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ spriteB = 20, xTB = xB + 4, xAttB = xB - 2, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 32 ){ spriteB = 20, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 19, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 27 ){ spriteB = 17, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 24 ){ xB = xB - 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB - 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB - 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ xB = xB - 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 12 ){ xB = xB - 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ spriteB = 37, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){ xB = xB - 1, spriteB = 37,  GOTOU = 2 , PlayAPCM(2, decapite, 0) ;  }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 37, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 59 ){ //decapiteR
          decapitated[1] = 0;
          xFB = xB + 3, xTB = xB + 2, xMB = xB + 2, xGB = xB + 4;
           if ( GOTOU == 0 && temps > reftempsB + 44 ){ Boccupe = 0, spriteB = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ spriteB = 19, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ spriteB = 20, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 34 ){ spriteB = 20, xTB = xB, xAttB = xB + 6, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 32 ){ spriteB = 20, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 19, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 27 ){ spriteB = 17, yAttB = yTB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 24 ){ xB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 22 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ xB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 19 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ xB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ xB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 13 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 12 ){ xB = xB + 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ spriteB = 37, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){ xB = xB + 1, spriteB = 37, GOTOU = 2 , PlayAPCM(2, decapite, 0) ;  }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 37, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 60 ){ //front
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          yGB = 20;
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ Boccupe = 0, pB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ spriteB = 35, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 36, xAttB = xB + 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){
              spriteB = 36, distance = xB - xA;
               if ( pA == 60 ){ GOTOU = 2 ; }
               if ( GOTOU == 0 && pA == 39 && distance < 9 ){
			SwordCling();
              };
              xFB = xB + 4, xAttB = xB - 3, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 35, yAttB = yFB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 34, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 61 ){ //frontR
          yGB = 20;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ Boccupe = 0, pB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 25 ){ spriteB = 35, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 17 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 36, xAttB = xB, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){
              spriteB = 36, distance = xA - xB;
               if ( pA == 61 ){ GOTOU = 2 ; }
               if ( GOTOU == 0 && pA == 41 && distance < 9 ){
			SwordCling();
              };
              xFB = xB, xAttB = xB + 7, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){
              PlayAPCM(2, epee, 0);
              songrogne = songrogne + 1;
               if ( songrogne > 6 ){ songrogne = 0 ; }
               if ( songrogne == 3 ){ PlayAPCM(3, grogne1, 0) ; }
               if ( songrogne == 6 ){ PlayAPCM(3, grogne2, 0) ; }
              spriteB = 35, yAttB = yFB, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 34, GOTOU = 2 ; }
      };

      if ( GOTOU == 0 && pB == 62 ){ //retourne
         xAttB = xB + 4, xFB = xB, xTB = xB, xMB = xB, xGB = xB;
         yAttB = 14 ;
         spriteB = 37;
          if ( GOTOU == 0 && temps > reftempsB + 21 ){ pB = 1, Boccupe = 0, sens = 9, spriteA = 0, spriteB = 0, couleurok = 0, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 14 ){ spriteB = 39, GOTOU = 2 ; }
          if ( GOTOU == 0 && temps > reftempsB + 7 ){ spriteB = 38, GOTOU = 2 ; }
         GOTOU = 2;
     };

       if ( GOTOU == 0 && pB == 63 ){ //retourneR
          xAttB = xB, xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yAttB = 14 ;
          spriteB = 37;
           if ( GOTOU == 0 && temps > reftempsB + 21 ){ pB = 0, Boccupe = 0, sens = 6, spriteA = 0, spriteB = 0, couleurok = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 7 ){ spriteB = 38, GOTOU = 2 ; }
          GOTOU = 2;
      };


       if ( GOTOU == 0 && pB == 64 ){ //vainqueur
          spriteB = 40;
          decapitated[1] = 1;
          blood[1] = 0;
          spritereculeB = 0;
          xAttB = xB + 4, yAttB = 14, yGB = 20;
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          //psgvol 0,0;
           if ( GOTOU == 0 && temps > reftempsB + 100 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 86 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ spriteB = 42, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ spriteB = 41, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 65 ){ //vainqueurR
          spriteB = 40;
          decapitated[1] = 1;
          blood[1] = 0;
          spritereculeB = 0;
          xAttB = xB, yAttB = 14, yGB = 20;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          //psgvol 0,0;
           if ( GOTOU == 0 && temps > reftempsB + 100 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 86 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 37 ){ spriteB = 42, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 18 ){ spriteB = 41, GOTOU = 2 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 66 ){ //vainqueurKO
          decapitated[1] = 1;
          blood[1] = 0;
          spritereculeB = 0;
          xAttB = xB + 4, yGB = 20, //yAttB = 14 ;
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;

           if ( GOTOU == 0 && temps > reftemps + 230 ){
              gnome = 1, reftemps = temps;
              xGNOME = 3;
              xSPRt = 270;
              GOTOU = 31;
          };

           if ( GOTOU == 0 && temps > reftempsB + 205 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 195 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 140 ){ spriteB = 42, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 123 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 105 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 100 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 75 ){ spriteB = 30, spriteA = 50, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 71 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 70 ){ spriteB = 31, spriteA = 49, xA = xA - 2, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 55 ){ spriteB = 30, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 36 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 35 ){
              distance = xB - xA;
               if ( GOTOU == 0 && distance < 5 ){ spriteB = 6, xB = xB + 1 ; }
               if ( GOTOU == 0 && distance > 5 ){ spriteB = 6, xB = xB - 1 ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 23 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){ spriteB = 37 ; }
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 67 ){ // vainqueurKOR
          decapitated[1] = 1;
          blood[1] = 0;
          spritereculeB = 0;
          xAttB = xB, yGB = 20, // yAttB = 14;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;

           if ( GOTOU == 0 && temps > reftemps + 230 ){
              gnome = 1, reftemps = temps;
              xGNOME = 3;
              xSPRt = 270;
              GOTOU = 31;
          };

           if ( GOTOU == 0 && temps > reftempsB + 205 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 195 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 140 ){ spriteB = 42, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 123 ){ spriteB = 41, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 105 ){ spriteB = 40, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 100 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 75 ){ spriteB = 30, spriteA = 50, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 71 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 70 ){ spriteB = 31, spriteA = 49, xA = xA + 2, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 55 ){ spriteB = 30, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 36 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 35 ){
              distance = xA - xB;
               if ( GOTOU == 0 && distance < 5 ){ spriteB = 6, xB = xB - 1 ; }
               if ( GOTOU == 0 && distance > 5 ){ spriteB = 6, xB = xB + 1 ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 23 ){ spriteB = 39, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 15 ){ spriteB = 38, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){ spriteB = 37 ; }
          GOTOU = 2;
      };

      ///                               ******degats B ******;
       if ( GOTOU == 0 && pB == 68 ){ //touche
          //psgvol 2,0;
          attenteB = 0;
          xAttB = xB + 4;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
          yFB = 15, yTB = 16, yMB = 18, yGB = 20;
           if ( GOTOU == 0 && pA == 54 ){ pB = 72, GOTOU = 24 ; }

            if ( GOTOU == 0 && serpent[1] == 0 ){ serpent[1] = 1, refserpent[1] = temps ; }

           if ( GOTOU == 0 && pA == 58 && decapitated[1] == 1 ){
              pB = 80, Boccupe = 1, reftempsB = temps;
              xSPRt = xB + 3, score[0] = score[0] + 250;
              blood[1] = 0, serpent[1] = 0;
              GOTOU = 33;
          };

           if ( GOTOU == 0 ){ life[1] = life[1] - 1 ; }

           if ( GOTOU == 0 && life[1] <= 0 ){
                blood[1] = 1, serpent[1] = 0;
              pB = 78, Boccupe = 1, reftempsB = temps, blood[1] = 1, GOTOU = 33;
          };
           if ( GOTOU == 0 ){
              pB = 69, Boccupe = 1, reftempsB = temps, decapitated[1] = 1;
              GOTOU = 2;
          };
      };

       if ( GOTOU == 0 && pB == 69 ){ // touche1
           if ( GOTOU == 0 && temps > reftempsB + 20 ){
              spriteB = 0, Boccupe = 0;
              blood[1] = 0;
              pB = 0, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ xB = xB + 1, blood[1] = 0, spriteB = 43, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB + 2, spriteB = 44, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){
              xB = xB + 1, spriteB = 43;
              choixTOUCHE = choixTOUCHE + 1;
               if ( choixTOUCHE > 3 ){ choixTOUCHE = 1 ; }
               if ( choixTOUCHE == 1 ){ PlayAPCM(2, touche, 0) ; }
               if ( choixTOUCHE == 2 ){ PlayAPCM(2, touche2, 0) ; }
               if ( choixTOUCHE == 3 ){ PlayAPCM(2, touche3, 0) ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 43, blood[1] = 1, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 70 ){ //toucheR
          attenteB = 0;
          xAttB = xB;
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
          yFB = 15, yTB = 16, yMB = 18, yGB = 20;
           if ( GOTOU == 0 && pA == 55 ){ pB = 74, GOTOU = 24 ; }

            if ( GOTOU == 0 && serpent[1] == 0 ){ serpent[1] = 1, refserpent[1] = temps ; }

           if ( GOTOU == 0 && pA == 59 && decapitated[1] == 1 ){
              pB = 81, Boccupe = 1, reftempsB = temps;

              xSPRt = xB + 3, score[0] = score[0] + 250;
              blood[1] = 0,serpent[1] = 0;
              GOTOU = 33;
          };

           if ( GOTOU == 0 ){ life[1] = life[1] - 1 ; }

           if ( GOTOU == 0 && life[1] <= 0 ){
			blood[1] = 1, serpent[1] = 0;
              pB = 79, Boccupe = 1, reftempsB = temps, blood[1] = 1, GOTOU = 33;
          };
           if ( GOTOU == 0 ){
              pB = 71, Boccupe = 1, reftempsB = temps, decapitated[1] = 1;
              GOTOU = 2;
          };
      };

       if ( GOTOU == 0 && pB == 71 ){ //touche1R
           if ( GOTOU == 0 && temps > reftempsB + 20 ){
              spriteB = 0, Boccupe = 0;
              blood[1] = 0;
              pB = 1, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ xB = xB - 1, blood[1] = 0, spriteB = 43, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ xB = xB - 2, spriteB = 44, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){
              xB = xB - 1, spriteB = 43;
              choixTOUCHE = choixTOUCHE + 1;
               if ( choixTOUCHE > 3 ){ choixTOUCHE = 1 ; }
               if ( choixTOUCHE == 1 ){ PlayAPCM(2, touche, 0) ; }
               if ( choixTOUCHE == 2 ){ PlayAPCM(2, touche2, 0) ; }
               if ( choixTOUCHE == 3 ){ PlayAPCM(2, touche3, 0) ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 43, blood[1] = 1, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 72 ){ //tombe
          //psgvol 2,0;
          attenteB = 0;
          xAttB = xB + 4;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;

          blood[1] = 1;
            if ( GOTOU == 0 && serpent[1] == 0 ){ serpent[1] = 1, refserpent[1] = temps ; }
          life[1] = life[1] - 1, score[0] = score[0] + 100;

           if ( GOTOU == 0 && pA == 32 ){ life[1] = life[1] + 1, blood[1] = 0, serpent[1] = 0, score[0] = score[0] - 100 ; }
           if ( GOTOU == 0 && life[1] <= 0 ){
                blood[1] = 1, serpent[1] = 0;
              pB = 78, Boccupe = 1, reftempsB = temps, blood[1] = 1;
               if ( GOTOU == 0  ){
				   if ( pA == 56 || pA == 54 ){ blood[1] = 0 ; }
			   ; }
              GOTOU = 33;
          };

           if ( GOTOU == 0  ){
			   if ( pA == 56 || pA == 54 ){ score[0] = score[0] + 150, blood[1] = 0 ; }
          };

           if ( GOTOU == 0 ){ pB = 73, Boccupe = 1, reftempsB = temps, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 73 ){ // tombe1
          attenteB = 0;
          xAttB = xB + 4;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
           if ( GOTOU == 0 && temps > reftempsB + 25 ){
              spriteB = 0, xB = xB + 1, pB = 0, Boccupe = 0, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 47, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ blood[1] = 0, xB = xB + 2, spriteB = 46, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){
              xB = xB + 2;
              PlayAPCM(2, tombe, 0) ;
              if ( pA == 54 ){ PlayAPCM(2, coupdepied, 0) ; }
              if ( pA == 56 ){ PlayAPCM(2, coupdetete, 0) ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 45, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 74 ){ //tombeR
          //psgvol 2,0;
          xAttB = xB;
          attenteB = 0;
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;

          blood[1] = 1;
            if ( GOTOU == 0 && serpent[1] == 0 ){ serpent[1] = 1, refserpent[1] = temps ; }
          life[1] = life[1] - 1, score[0] = score[0] + 100;

           if ( GOTOU == 0 && pA == 34 ){ life[1] = life[1] + 1, blood[1] = 0, serpent[1] = 0, score[0] = score[0] - 100 ; }
           if ( GOTOU == 0 && life[1] <= 0 ){
                blood[1] = 1, serpent[1] = 0;
              pB = 79, Boccupe = 1, reftempsB = temps, blood[1] = 1;
               if ( GOTOU == 0  ){
				   if ( pA == 57 || pA == 55 ){ blood[1] = 0 ; }
			    ; }
              GOTOU = 33;
          };

           if ( GOTOU == 0  ){
			   if ( pA == 57 || pA == 55 ){ score[0] = score[0] + 150, blood[1] = 0 ; }
		   ; }

           if ( GOTOU == 0 ){ pB = 75, Boccupe = 1, reftempsB = temps, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 75 ){ // tombe1R
          xAttB = xB;
          attenteB = 0;
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
           if ( GOTOU == 0 && temps > reftempsB + 25 ){
              spriteB = 0, xB = xB - 1, pB = 1, Boccupe = 0, GOTOU = 2;
          };
           if ( GOTOU == 0 && temps > reftempsB + 16 ){ spriteB = 47, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 9 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 8 ){ blood[1] = 0, xB = xB - 2, spriteB = 46, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){
              xB = xB - 2;
              PlayAPCM(2, tombe, 0) ;
              if ( pA == 55 ){ PlayAPCM(2, coupdepied, 0) ; }
              if ( pA == 57 ){ PlayAPCM(2, coupdetete, 0) ; }
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 45, GOTOU = 2 ; }
      };


      //                               bruit des epees  et decapitations loupees;
       if ( GOTOU == 0 && sens == 6 ){

           if ( GOTOU == 0 && pB == 76 ){//clingD
               if ( GOTOU == 0 && pA == 58 && decapitated[1] == 0 ){ pB = 68, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && pA == 50 ){ pB = 68, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }

               if ( GOTOU == 0 ){ pB = 43, GOTOU = 2 ; }
          };

           if ( GOTOU == 0 && pB == 77 ){//clingH

              pB = 39, GOTOU = 2;
          };

      };

       if ( GOTOU == 0 && sens == 9 ){

           if ( GOTOU == 0 && pB == 76 ){//clingD

               if ( GOTOU == 0 && pA == 59 && decapitated[1] == 0 ){ pB = 70, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }
               if ( GOTOU == 0 && pA == 51 ){ pB = 70, Boccupe = 1, reftempsB = temps, GOTOU = 24 ; }

               if ( GOTOU == 0 ){ pB = 45, GOTOU = 2 ; }
          };

           if ( GOTOU == 0 && pB == 77 ){//clingH

              pB = 41, GOTOU = 2;
          };

      };

      ///                                   *************************************;
      ///                                   **********gestion de la mort*********;
      ///                                   *************************************;
      //mortB ;
       if ( GOTOU == 33 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && pB == 78 ){ //mort
          xFB = xB, xTB = xB, xMB = xB, xGB = xB;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ blood[1] = 0, spriteB = 48, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ spriteB = 9, GOTOU = 2 , PlayAPCM(1, mortKO, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 9, pA = 66, Aoccupe = 1, reftemps = temps, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 79 ){ //mortR
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;
           if ( GOTOU == 0 && temps > reftempsB + 35 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ blood[1] = 0, spriteB = 48, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 2 ){  GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 1 ){ spriteB = 9, GOTOU = 2 , PlayAPCM(1, mortKO, 0) ; }
           if ( GOTOU == 0 && temps >= reftempsB ){ spriteB = 9, pA = 67, Aoccupe = 1, reftemps = temps, GOTOU = 2 ; }
      };

       if ( GOTOU == 0 && pB == 80 ){ //mortdecap
           if ( GOTOU == 0 && temps > reftempsB + 90 ){ spriteB = 54, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 80 ){ spriteB = 53, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 70 ){ bloodDecap[1] = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 60 ){ bloodDecap[1] = 6, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 50 ){ bloodDecap[1] = 5, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ bloodDecap[1] = 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 52, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ bloodDecap[1] = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ bloodDecap[1] = 3, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ bloodDecap[1] = 2, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ bloodDecap[1] = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){
              spriteB = 51, pA = 64, Aoccupe = 1, reftemps = temps, PlayAPCM(1, mortdecap, 0) ;
          };
          GOTOU = 2;
      };

       if ( GOTOU == 0 && pB == 81 ){ //mortdecapR
           if ( GOTOU == 0 && temps > reftempsB + 90 ){ spriteB = 54, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 80 ){ spriteB = 53, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 70 ){ bloodDecap[1] = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 60 ){ bloodDecap[1] = 6, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 50 ){ bloodDecap[1] = 5, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 40 ){ bloodDecap[1] = 4, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 30 ){ spriteB = 52, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 20 ){ bloodDecap[1] = 0, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 14 ){ bloodDecap[1] = 3, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 10 ){ bloodDecap[1] = 2, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 6 ){ bloodDecap[1] = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 5 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps >= reftempsB ){
              spriteB = 51, pA = 65, Aoccupe = 1, reftemps = temps, PlayAPCM(1, mortdecap, 0) ;
          };
          GOTOU = 2;
      };


      ///                   ***************************************;
      ///                   ***********   Sorceror    **************;
      ///                   ***************************************;

       if ( GOTOU == 0 && pB == 82 ){

          gnome = 0;
          xFB = xB + 4, xTB = xB + 4, xMB = xB + 4, xGB = xB + 4;

           if ( GOTOU == 0 && temps > reftempsB + 291 ){ spriteB = 55, reftempsB = temps, GOTOU = 2 ; }


           if ( GOTOU == 0 && temps > reftempsB + 290 ){ xAttB = 3, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 280 ){ xAttB = 27, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 270 ){ xAttB = 24, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 260 ){ xAttB = 21, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 250 ){ xAttB = 18, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 240 ){ xAttB = 15, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 230 ){ xAttB = 12, spriteB = 55, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 220 ){ xAttB = 9, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 211 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 210 ){
              spriteB = 56, yAttB = 16;
              PlayAPCM(2, feu, 0);
              GOTOU = 2;
          };

           if ( GOTOU == 0 && temps > reftempsB + 130 ){ xAttB = 1, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 120 ){ xAttB = 27, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 110 ){ xAttB = 24, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 100 ){ xAttB = 21, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 90 ){ xAttB = 18, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 80 ){ xAttB = 15, yAttB = 20, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 70 ){ xAttB = 12, spriteB = 55, yAttB = 18, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 60 ){ xAttB = 9, GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 51 ){ GOTOU = 2 ; }
           if ( GOTOU == 0 && temps > reftempsB + 50 ){
              spriteB = 56;
              PlayAPCM(2, feu, 0);
              GOTOU = 2;
          };
           if ( GOTOU == 0 && temps >= reftempsB ){
              spriteB = 55;
              xAttB = xB, yAttB = 16, yGB = 20;
              blood[1] = 0;
          };
      };



    ///                   ***************************************;
    ///                   ***********   Collision   **************;
    ///                   ***************************************;
    //colision ;
      if ( GOTOU == 2 ){ GOTOU = 0 ; }

     if ( GOTOU == 0 && sens == 9 ){ GOTOU = 3 ; }

     if ( GOTOU == 0 && pA == 24 && pB == 32 ){ GOTOU = 28 ; }
     if ( GOTOU == 0 && pB == 24 && pA == 32 ){ GOTOU = 28 ; }



     if ( GOTOU == 0 ){

      //                               pour empecher que A entre dans B;

	if ( level[0] == 6 || pA == 32 || pA == 58 ){
        	distance = xB - xA;
         if ( distance < 4 ){ xA = xA - 1 ; }
	};

      //                                pour empecher que B entre dans A;

      if ( level[1] == 4 || pB == 32 || pB == 58 ){
	distance = xB - xA;
        if ( distance < 4 ){ xB = xB + 1 ; }
     };


      //                               garder la distance debout;
      if ( pB == 0 && pA == 0 ){
        distance = xB - xA;
         if ( distance < 4 ){ xA = xA - 1, xB = xB + 1 ; }
     };

    };

    //                               sortie du cadre;
    //sortiecadre ;
     if ( GOTOU == 28 ){ GOTOU = 0 ; }

     if ( GOTOU == 0 && entrance == 1 ){
         if ( xA < 0 ){ xA = 0 ; }
         if ( xB < 0 ){ xB = 0 ; }
         if ( xA > 40 ){ xA = 40 ; }
         if ( xB > 40 ){ xB = 40 ; }
        GOTOU = 30;
    };

     if ( GOTOU == 0 ){
         if ( sortieA == 1 || sortieB == 1 ){
             if ( xA < 0 ){ xA = 0 ; }
             if ( xB > 40 ){ xB = 40 ; }
            GOTOU = 30;
        };
    };

     if ( GOTOU == 0 && entrance == 0 ){
         if ( xA < 5 ){ xA = 5 ; }
         if ( xA > 32 ){ xA = 32 ; }
         if ( xB < 5 ){ xB = 5 ; }
         if ( xB > 32 ){ xB = 32 ; }
    };


     if ( GOTOU == 0 ){ GOTOU = 30 ; }


     ///                   ***************************************;
     ///                   ***********   COLISION INVERSE ********;
     ///                   ***************************************;

      if ( GOTOU == 0 && sens == 6 ){ GOTOU = 31 ; }

     //colisionR ;
      if ( GOTOU == 3 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 && pA == 25 && pB == 34 ){ GOTOU = 27 ; }
      if ( GOTOU == 0 && pB == 25 && pA == 34 ){ GOTOU = 27 ; }

     //                               pour empecher que A entre dans B (inverse);

      if ( GOTOU == 0 ){
          if ( level[0] == 4 || pA == 34 || pA == 59 ){
             distance = xA - xB;
              if ( distance < 4 ){ xA = xA + 1 ; }
         };
     };


     ///                                pour empecher que B entre dans A (inverse);

      if ( GOTOU == 0 ){
          if ( level[1] == 6 || pB == 34 || pB == 59 ){
             distance = xA - xB;
              if ( distance < 4 ){ xB = xB - 1 ; }
         };
     };

     //                               garder la distance debout;
      if ( GOTOU == 0 && pB == 1 && pA == 1 ){
         distance = xA - xB;
          if ( distance < 4 ){ xA = xA + 1, xB = xB - 1 ; }
     };


     //sortiecadreR ;
      if ( GOTOU == 27 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 ){
          if ( sortieA == 1 || sortieB == 1 || enterSorceror == 1 ){
              if ( xB < 0 ){ xB = 0 ; }
              if ( xA > 40 ){ xA = 40 ; }
             GOTOU = 30;
         };
     };

      if ( GOTOU == 0 ){
          if ( xA < 5 ){ xA = 5 ; }
          if ( xA > 31 ){ xA = 32 ; }
          if ( xB < 5 ){ xB = 5 ; }
          if ( xB > 32 ){ xB = 32 ; }
     };


     ///                       ****************************************;
     ///                       ********* les tetes volantes ! *********;
     ///                       ****************************************;
     //tetesvolantes ;
      if ( GOTOU == 30 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 ){
           if ( sortieA == 1 || sortieB == 1 ){ GOTOU = 31 ; }
      };

       if ( GOTOU == 0 ){

           if ( xA > 19 && xB > 20 ){ GOTOU = 29 ; }

           if ( GOTOU == 0 ){
               if ( pA == 80 || pA == 81 ){


                   if ( GOTOU == 0 && temps > reftemps + 125 ){
                      gnome = 1, reftemps = temps;
                      xGNOME = 3;
                      GOTOU = 12;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 56 ){ ySPRt = 164, xSPRt = xA + 16, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 52 ){ ySPRt = 157, xSPRt = xA + 15, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 48 ){ ySPRt = 159, xSPRt = xA + 14, numerotete = 1, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 45 ){ ySPRt = 157, xSPRt = xA + 13, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 44 ){
                       if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 157, xSPRt = xA + 13, numerotete = 6, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 40 ){ ySPRt = 159, xSPRt = xA + 12, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 36 ){ ySPRt = 159, xSPRt = xA + 11, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 32 ){ ySPRt = 159, xSPRt = xA + 10, numerotete = 3, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 29 ){ ySPRt = 159, xSPRt = xA + 9, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 28 ){
                       if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 159, xSPRt = xA + 9, numerotete = 2, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 24 ){ ySPRt = 139, xSPRt = xA + 8, numerotete = 1, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 20 ){ ySPRt = 125, xSPRt = xA + 7, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 16 ){ ySPRt = 125, xSPRt = xA + 6, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 12 ){ ySPRt = 118, xSPRt = xA + 5, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 8 ){ ySPRt = 111, xSPRt = xA + 4, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 4 ){ ySPRt = 111, xSPRt = xA + 3, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps >= reftemps ){ ySPRt = 111, xSPRt = xA + 2, numerotete = 1, GOTOU = 31 ; }

              };
          };


           if ( GOTOU == 0 ){
               if ( pB == 80 || pB == 81 ){

                   if ( GOTOU == 0 && temps > reftemps + 125 ){
                      gnome = 1, reftemps = temps;
                      xGNOME = 3;
                      GOTOU = 12;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 56 ){ ySPRt = 164, xSPRt = xB + 16, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 52 ){ ySPRt = 157, xSPRt = xB + 15, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 48 ){ ySPRt = 159, xSPRt = xB + 14, numerotete = 1, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 45 ){ ySPRt = 157, xSPRt = xB + 13, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 44 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 157, xSPRt = xB + 13, numerotete = 6, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 40 ){ ySPRt = 159, xSPRt = xB + 12, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 36 ){ ySPRt = 159, xSPRt = xB + 11, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 32 ){ ySPRt = 159, xSPRt = xB + 10, numerotete = 3, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 29 ){ ySPRt = 159, xSPRt = xB + 9, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 28 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 159, xSPRt = xB + 9, numerotete = 2, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 24 ){ ySPRt = 139, xSPRt = xB + 8, numerotete = 1, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 20 ){ ySPRt = 125, xSPRt = xB + 7, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 16 ){ ySPRt = 125, xSPRt = xB + 6, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 12 ){ ySPRt = 118, xSPRt = xB + 5, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 8 ){ ySPRt = 111, xSPRt = xB + 4, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 4 ){ ySPRt = 111, xSPRt = xB + 3, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps >= reftemps ){ ySPRt = 111, xSPRt = xB + 2, numerotete = 1, GOTOU = 31 ; }
                  GOTOU = 31;
              };
          };

          //teteagauche ;
           if ( GOTOU == 29 ){ GOTOU = 0 ; }

           if ( GOTOU == 0 ){
               if ( pA == 80 || pA == 81 ){

                   if ( GOTOU == 0 && temps > reftemps + 125 ){
                      gnome = 1, reftemps = temps;
                      xGNOME = 3;
                      GOTOU = 12;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 56 ){ ySPRt = 164, xSPRt = xA - 12, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 52 ){ ySPRt = 157, xSPRt = xA - 11, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 48 ){ ySPRt = 159, xSPRt = xA - 10, numerotete = 1, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 45 ){ ySPRt = 157, xSPRt = xA - 9, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 44 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 157, xSPRt = xA - 9, numerotete = 6, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 40 ){ ySPRt = 159, xSPRt = xA - 8, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 36 ){ ySPRt = 159, xSPRt = xA - 7, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 32 ){ ySPRt = 159, xSPRt = xA - 6, numerotete = 3, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 29 ){ ySPRt = 159, xSPRt = xA - 5, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 28 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 159, xSPRt = xA - 5, numerotete = 2, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 24 ){ ySPRt = 139, xSPRt = xA - 4, numerotete = 1, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 20 ){ ySPRt = 125, xSPRt = xA - 3, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 16 ){ ySPRt = 125, xSPRt = xA - 2, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 12 ){ ySPRt = 118, xSPRt = xA - 1, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 8 ){ ySPRt = 111, xSPRt = xA, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 4 ){ ySPRt = 111, xSPRt = xA + 1, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps >= reftemps ){ ySPRt = 111, xSPRt = xA + 2, numerotete = 1, GOTOU = 31 ; }

              };
          };

           if ( GOTOU == 0 ){
               if ( pB == 80 || pB == 81 ){

                   if ( GOTOU == 0 && temps > reftemps + 125 ){
                      gnome = 1, reftemps = temps;
                      xGNOME = 3;
                      GOTOU = 12;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 56 ){ ySPRt = 164, xSPRt = xB - 14, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 52 ){ ySPRt = 157, xSPRt = xB - 13, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 48 ){ ySPRt = 159, xSPRt = xB - 12, numerotete = 1, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 45 ){ ySPRt = 157, xSPRt = xB - 11, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 44 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 157, xSPRt = xB - 11, numerotete = 6, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 40 ){ ySPRt = 159, xSPRt = xB - 10, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 36 ){ ySPRt = 159, xSPRt = xB - 9, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 32 ){ ySPRt = 159, xSPRt = xB - 8, numerotete = 3, GOTOU = 31 ; }

                   if ( GOTOU == 0 && temps > reftemps + 29 ){ ySPRt = 159, xSPRt = xB - 7, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 28 ){
                        if ( xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                      ySPRt = 159, xSPRt = xB - 7, numerotete = 2, GOTOU = 31;
                  };

                   if ( GOTOU == 0 && temps > reftemps + 24 ){ ySPRt = 139, xSPRt = xB - 6, numerotete = 1, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 20 ){ ySPRt = 125, xSPRt = xB - 5, numerotete = 6, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 16 ){ ySPRt = 125, xSPRt = xB - 4, numerotete = 5, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 12 ){ ySPRt = 118, xSPRt = xB - 3, numerotete = 4, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 5 ){ ySPRt = 111, xSPRt = xB - 2, numerotete = 3, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps > reftemps + 4 ){ ySPRt = 111, xSPRt = xB - 1, numerotete = 2, GOTOU = 31 ; }
                   if ( GOTOU == 0 && temps >= reftemps ){ ySPRt = 111, xSPRt = xB, numerotete = 1, GOTOU = 31 ; }

              };
          };


      };


      ///                   *********************************;
      ///                   *******  PASSAGE DU GNOME *******;
      ///                   *********************************;

      //gnome ;
       if ( GOTOU == 12 ){ GOTOU = 0 ; }
       if ( gnome == 1 ){

          numerognome = 4;
           if ( GOTOU == 0 && temps > reftemps + 6 ){ numerognome = 1 ; }
           if ( GOTOU == 0 && temps > reftemps + 12 ){ numerognome = 2 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ numerognome = 3 ; }

           if ( GOTOU == 0 && temps > reftemps + 24 ){ reftemps = temps, GOTOU = 31 ; }
           if ( GOTOU == 0 && temps > reftemps + 19 ){ GOTOU = 31 ; }
           if ( GOTOU == 0 && temps > reftemps + 18 ){ xGNOME = xGNOME + 1, GOTOU = 31 ; }
           if ( GOTOU == 0 && temps > reftemps + 7 ){ GOTOU = 31 ; }
           if ( GOTOU == 0 && temps > reftemps + 6 ){ xGNOME = xGNOME + 1, GOTOU = 31 ; }



      };



     ///                   **********************************************;
     ///                   ************      Display        ***********;
     ///                   **********************************************;

      if ( GOTOU == 31 ){ GOTOU = 0 ; }


      if ( GOTOU == 0 ){ 

	///TODO DRAW BG


         //bottom screen messages
           if ( partie == 1 && demo == 0 )
                {
                	//TODO "ONE PLAYER" @ 240,384

                };
           if ( partie == 2 )
                {
                            //TODO "TWO PLAYER" @ 240,384

                };
           if ( demo == 1 )
                {
                                	//TODO "DEMONSTRATION" @ 216,384

                };



      if ( transition == 1 ){
          if ( temps > reftemps + 60 ){ transition = 0, transitionfinie = 1 ; }
      };


         if ( sorceror == 1 ){

              ///hide life points;
/*
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 89, 42, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 0;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);

				position.x = 550;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);


              /// cache Drax;
                cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 32, 24, 32, 0, 0, 0, 0);
				position.x = 228;
                position.y = 190;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);

				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 52, 16, 32, 0, 0, 0, 0);
				position.x = 218;
                position.y = 214;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);

              ///cache marianna;
              if ( marianna == 1 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 27, 34, 32, 0, 0, 0, 0);
				position.x = 373;
                position.y = 190;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);

                cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 34, 6, 32, 0, 0, 0, 0);
				position.x = 370;
                position.y = 224;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
             };
          };

*/
	};


    ///                               affichage du temps;


      if ( GOTOU == 0 ){
		if((pA >= 78 && pA <= 81) || (pB >= 78 && pB <= 81)){ GOTOU = 13; }

	}else{

	      	if ( temps > chrono + 60 ){ timer = timer - 1, chrono = temps ; }

	          if ( timer < 1 || timer > 90 ){
        		timer = 0;
	              	if ( partie == 2 ){ sortieA = 1, sortieB = 1, tempsfini = 1, Aoccupe = 0, Boccupe = 0 ; }
        	  ; }
	};


      if ( GOTOU == 13 ){ GOTOU = 0 ; }


	if ( GOTOU == 0 ){

            if ( partie == 1 ){
                //sprintf(texteIA, "0%d", IA);
                        };

        	if ( partie == 2 ){

                if ( timer >= 10 ){
                        //sprintf(texteIA, "%d", timer);
                        };

                 if ( timer < 10 ){
                        //sprintf(texteIA, "0%d", timer);
                        };

            };

                position.x = 304;
                position.y = 116;
                SDL_Surface *surfacetexteIA = TTF_RenderText_Solid(police,texteIA,fontColor) ;
                SDL_BlitSurface(surfacetexteIA, NULL, ecran, &position);

        };



     ///                                  affichage du score;

	if ( GOTOU == 0 ){

      	if ( score[0] > 99999 ){ score[0] = 0 ; }
      	if ( score[1] > 99999 ){ score[1] = 0 ; }

        if ( score[0] == 0 ){ }//sprintf(texteSCORE, "00000"); }

        if ( score[0] > 99 && score[0] < 999 ){ }//sprintf(texteSCORE, "00%d", score[0]); }

        if ( score[0] > 999 && score[0] < 9999 ){ }//sprintf(texteSCORE, "0%d", score[0]); }


        if ( score[0] > 9999 && score[0] < 99999 ){ }//sprintf(texteSCORE, "%d", score[0]); }

        position.x = 188;
        position.y = 116;
        SDL_Surface *surfacescore[0] = TTF_RenderText_Solid(police,texteSCORE,fontColor) ;
        SDL_BlitSurface(surfacescore[0], NULL, ecran, &position);


        if ( score[1] == 0 ){ }//sprintf(texteSCORE, "00000"); }

        if ( score[1] > 99 && score[1] < 999 ){ }//sprintf(texteSCORE, "00%d", score[1]); }

        if ( score[1] > 999 && score[1] < 9999 ){ }//sprintf(texteSCORE, "0%d", score[1]); }

        if ( score[1] > 9999 && score[1] < 99999 ){ }//sprintf(texteSCORE, "%d", score[1]); }

        position.x = 380;
        position.y = 116;
        SDL_Surface *surfacescore[1] = TTF_RenderText_Solid(police,texteSCORE,fontColor) ;
        SDL_BlitSurface(surfacescore[1], NULL, ecran, &position);





       ///                                  effacement des points de vie;

       if ( life[0] < 12 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 75;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 11 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 61;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 10 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 44;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 9 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 30;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 8 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 11;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 7 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 0;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 6 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 75;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 5 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 61;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 4 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 44;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
       if ( life[0] < 3 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 30;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[0] < 2 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 11;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
       if ( life[0] == 0 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 89, 42, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 0;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }


        if ( life[1] < 12 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 627;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 11 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 613;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 10 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 596;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 9 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 582;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 8 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 563;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 7 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 23, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 550;
                position.y = 19;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 6 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 629;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 5 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 615;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 4 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 598;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
       if ( life[1] < 3 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 584;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
      if ( life[1] < 2 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 14, 19, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 566;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }
       if ( life[1] == 0 ){
				cacheNOIR = SDL_CreateRGBSurface(SDL_HWSURFACE, 89, 42, 32, 0, 0, 0, 0);
				SDL_FillRect(cacheNOIR, NULL, SDL_MapRGB(ecran->format, 0, 0, 0));
				position.x = 550;
                position.y = 0;
				SDL_BlitSurface(cacheNOIR, NULL, ecran, &position);
       }





    };


     ///                       ****************************************;
     ///                       *********** affichage sprite A**********;
     ///                       ****************************************;

      if ( GOTOU == 0 ){

        if ( spriteA == 0 ){ imageA = deboutA; }
        if ( spriteA == 1 ){ imageA = attente1A; }
        if ( spriteA == 2 ){ imageA = attente2A; }
        if ( spriteA == 3 ){ imageA = attente3A; }
        if ( spriteA == 4 ){ imageA = marche1A; }
        if ( spriteA == 5 ){ imageA = marche2A; }
        if ( spriteA == 6 ){ imageA = marche3A; }
        if ( spriteA == 7 ){ imageA = saut1A; }
        if ( spriteA == 8 ){ imageA = saut2A; }
        if ( spriteA == 9 ){ imageA = assis1A; }
        if ( spriteA == 10 ){ imageA = assis2A; }
        if ( spriteA == 11 ){ imageA = roulade1A; }
        if ( spriteA == 12 ){ imageA = roulade2A; }
        if ( spriteA == 13 ){ imageA = roulade3A; }
        if ( spriteA == 14 ){ imageA = roulade4A; }
        if ( spriteA == 15 ){ imageA = roulade5A; }
        if ( spriteA == 16 ){ imageA = roulade6A; }
        if ( spriteA == 17 ){ imageA = protegeHautA; }
        if ( spriteA == 18 ){ imageA = protegeDevantA; }
        if ( spriteA == 19 ){ imageA = cou2A; }
        if ( spriteA == 20 ){ imageA = cou3A; }
        if ( spriteA == 21 ){ imageA = devant1A; }
        if ( spriteA == 22 ){ imageA = devant2A; }
        if ( spriteA == 23 ){ imageA = devant3A; }
        if ( spriteA == 24 ){ imageA = genou1A; }
        if ( spriteA == 25 ){ imageA = genou3A; }
        if ( spriteA == 26 ){ imageA = toile1A; }
        if ( spriteA == 27 ){ imageA = toile2A; }
        if ( spriteA == 28 ){ imageA = toile3A; }
        if ( spriteA == 29 ){ imageA = toile4A; }
        if ( spriteA == 30 ){ imageA = pied1A; }
        if ( spriteA == 31 ){ imageA = pied2A; }
        if ( spriteA == 32 ){ imageA = couptete1A; }
        if ( spriteA == 33 ){ imageA = couptete2A; }
        if ( spriteA == 34 ){ imageA = front1A; }
        if ( spriteA == 35 ){ imageA = front2A; }
        if ( spriteA == 36 ){ imageA = front3A; }
        if ( spriteA == 37 ){ imageA = retourne1A; }
        if ( spriteA == 38 ){ imageA = retourne2A; }
        if ( spriteA == 39 ){ imageA = retourne3A; }
        if ( spriteA == 40 ){ imageA = vainqueur1A; }
        if ( spriteA == 41 ){ imageA = vainqueur2A; }
        if ( spriteA == 42 ){ imageA = vainqueur3A; }
        if ( spriteA == 43 ){ imageA = touche2A; }
        if ( spriteA == 44 ){ imageA = touche1A; }
        if ( spriteA == 45 ){ imageA = tombe1A; }
        if ( spriteA == 46 ){ imageA = tombe2A; }
        if ( spriteA == 47 ){ imageA = tombe3A; }
        if ( spriteA == 48 ){ imageA = mort2A; }
        if ( spriteA == 49 ){ imageA = mort3A; }
        if ( spriteA == 50 ){ imageA = mort4A; }
        if ( spriteA == 51 ){ imageA = decap1A; }
        if ( spriteA == 52 ){ imageA = decap2A; }
        if ( spriteA == 53 ){ imageA = decap3A; }
        if ( spriteA == 54 ){ imageA = decap4A; }




          if ( xA < 39 ){

              if ( sens == 6 ){ // affichage normal;

                position.x = ((xA - 1) * 16 ) , position.y = ((yFA - 2) * 16) ;

              ////  SDL_BlitSurface(imageA, NULL, ecran, &position);


             };


              if ( sens == 9 ){ // affichage inverse;

                    position.x = ((xA - 5) * 16 ) , position.y = ((yFA - 2) * 16) ;

                   ///// imageAflip = rotozoomSurfaceXY(imageA, 0 , -1 ,1 , 0); // surface, angle,zoomx, zoomy, lissage
                   //// SDL_BlitSurface(imageAflip , NULL, ecran, &position);
                   //// SDL_FreeSurface(imageAflip);

             };

         };


     };

     ///                       ****************************************;
     ///                       *********** affichage sprite B**********;
     ///                       ****************************************;


      if ( GOTOU == 0 ){


		if ( spriteB == 0 ){ imageB = deboutB; }
        if ( spriteB == 1 ){ imageB = attente1B; }
        if ( spriteB == 2 ){ imageB = attente2B; }
        if ( spriteB == 3 ){ imageB = attente3B; }
        if ( spriteB == 4 ){ imageB = marche1B; }
        if ( spriteB == 5 ){ imageB = marche2B; }
        if ( spriteB == 6 ){ imageB = marche3B; }
        if ( spriteB == 7 ){ imageB = saut1B; }
        if ( spriteB == 8 ){ imageB = saut2B; }
        if ( spriteB == 9 ){ imageB = assis1B; }
        if ( spriteB == 10 ){ imageB = assis2B; }
        if ( spriteB == 11 ){ imageB = roulade1B; }
        if ( spriteB == 12 ){ imageB = roulade2B; }
        if ( spriteB == 13 ){ imageB = roulade3B; }
        if ( spriteB == 14 ){ imageB = roulade4B; }
        if ( spriteB == 15 ){ imageB = roulade5B; }
        if ( spriteB == 16 ){ imageB = roulade6B; }
        if ( spriteB == 17 ){ imageB = protegeHautB; }
        if ( spriteB == 18 ){ imageB = protegeDevantB; }
        if ( spriteB == 19 ){ imageB = cou2B; }
        if ( spriteB == 20 ){ imageB = cou3B; }
        if ( spriteB == 21 ){ imageB = devant1B; }
        if ( spriteB == 22 ){ imageB = devant2B; }
        if ( spriteB == 23 ){ imageB = devant3B; }
        if ( spriteB == 24 ){ imageB = genou1B; }
        if ( spriteB == 25 ){ imageB = genou3B; }
        if ( spriteB == 26 ){ imageB = toile1B; }
        if ( spriteB == 27 ){ imageB = toile2B; }
        if ( spriteB == 28 ){ imageB = toile3B; }
        if ( spriteB == 29 ){ imageB = toile4B; }
        if ( spriteB == 30 ){ imageB = pied1B; }
        if ( spriteB == 31 ){ imageB = pied2B; }
        if ( spriteB == 32 ){ imageB = couptete1B; }
        if ( spriteB == 33 ){ imageB = couptete2B; }
        if ( spriteB == 34 ){ imageB = front1B; }
        if ( spriteB == 35 ){ imageB = front2B; }
        if ( spriteB == 36 ){ imageB = front3B; }
        if ( spriteB == 37 ){ imageB = retourne1B; }
        if ( spriteB == 38 ){ imageB = retourne2B; }
        if ( spriteB == 39 ){ imageB = retourne3B; }
        if ( spriteB == 40 ){ imageB = vainqueur1B; }
        if ( spriteB == 41 ){ imageB = vainqueur2B; }
        if ( spriteB == 42 ){ imageB = vainqueur3B; }
        if ( spriteB == 43 ){ imageB = touche2B; }
        if ( spriteB == 44 ){ imageB = touche1B; }
        if ( spriteB == 45 ){ imageB = tombe1B; }
        if ( spriteB == 46 ){ imageB = tombe2B; }
        if ( spriteB == 47 ){ imageB = tombe3B; }
        if ( spriteB == 48 ){ imageB = mort2B; }
        if ( spriteB == 49 ){ imageB = mort3B; }
        if ( spriteB == 50 ){ imageB = mort4B; }
        if ( spriteB == 51 ){ imageB = decap1B; }
        if ( spriteB == 52 ){ imageB = decap2B; }
        if ( spriteB == 53 ){ imageB = decap3B; }
        if ( spriteB == 54 ){ imageB = decap4B; }

        if ( spriteB == 55 ){ imageB = drax1B; }
        if ( spriteB == 56 ){ imageB = drax2B; }
        if ( spriteB == 57 ){ imageB = mariannaB; }






          if ( xB < 39 ){

              if ( sens == 6 ){ // affichage normal

                    position.x = (xB - 5) * 16, position.y = ((yFB - 2) * 16);

                    SDL_BlitSurface(imageB, NULL, ecran, &position);
             };


              if ( sens == 9 ){ // affichage inverse

					//a4 = (xB + 3) * 16, b4 = ((yFB - 2) * 16);
			        position.x = ( xB-1 )  * 16, position.y = ((yFB - 2) * 16);

                    imageBflip = rotozoomSurfaceXY(imageB, 0 , -1 ,1 , 0); // surface, angle,zoomx, zoomy, lissage
                    SDL_BlitSurface(imageBflip , NULL, ecran, &position);
                    SDL_FreeSurface(imageBflip);

             };


         };


     };

      ///                           ***********************************************;
      ///                           re-affichage du spriteA en cas de chevauchement;



       if ( GOTOU == 0 ){
           if ( pA == 78 || pA == 79 || pA == 80 || pA == 81 || pA == 50 || pA == 51 ){


               if ( xA < 39 ){
                                if ( sens == 6 ){ // affichage normal;
                                    position.x = ((xA - 1) * 16 ) , position.y = ((yFA - 2) * 16) ;
                                    SDL_BlitSurface(imageA, NULL, ecran, &position);
                                    };

                                if ( sens == 9 ){ // affichage inverse;
                                    position.x = ((xA - 5) * 16 ) , position.y = ((yFA - 2) * 16) ;

                                    imageAflip = rotozoomSurfaceXY(imageA, 0 , -1 ,1 , 0); // surface, angle,zoomx, zoomy, lissage
                                    SDL_BlitSurface(imageAflip , NULL, ecran, &position);
                                    SDL_FreeSurface(imageAflip);
                                    };
                            };
            };
      };



     ///                       ****************************************;
     ///                       ********* affichage du sang ************;
     ///                       ****************************************;

     //                    giclees de sang de decapitation;

      if ( GOTOU == 0 ){
           if ( pA == 80 || pA == 81 ){
               if ( bloodDecap[0] == 1 ){
                  position.x = (xA * 16) + 3, position.y = (yFA - 1) * 16;
                  SDL_BlitSurface(gicle1, NULL, ecran, &position);
              };
               if ( bloodDecap[0] == 2 ){
                  position.x = (xA * 16) + 3, position.y = (yFA - 1) * 16;
                  SDL_BlitSurface(gicle2, NULL, ecran, &position);
              };
               if ( bloodDecap[0] == 3 ){
                  position.x = (xA * 16) + 3, position.y = (yFA - 1) * 16;
                  SDL_BlitSurface(gicle3, NULL, ecran, &position);
              };

               if ( bloodDecap[0] == 4 ){
                  position.x = (xA * 16) + 2, position.y = ((yMA - 2) * 16) - 2;
                  SDL_BlitSurface(gicle1, NULL, ecran, &position);
              };
               if ( bloodDecap[0] == 5 ){
                  position.x = (xA * 16) + 2, position.y = ((yMA - 2) * 16) - 2;
                  SDL_BlitSurface(gicle2, NULL, ecran, &position);
              };
               if ( bloodDecap[0] == 6 ){
                  position.x = (xA * 16) + 2, position.y = ((yMA - 2) * 16) - 2;
                  SDL_BlitSurface(gicle3, NULL, ecran, &position);
              };
          };

           if ( pB == 80 || pB == 81 ){
               if ( bloodDecap[1] == 1 ){
                  position.x = (xB * 16) + 3, position.y = (yFB - 1) * 16;
                  SDL_BlitSurface(gicle1, NULL, ecran, &position);
              };
               if ( bloodDecap[1] == 2 ){
                  position.x = (xB * 16) + 3, position.y = (yFB - 1) * 16;
                  SDL_BlitSurface(gicle2, NULL, ecran, &position);
              };
               if ( bloodDecap[1] == 3 ){
                  position.x = (xB * 16) + 3, position.y = (yFB - 1) * 16;
                  SDL_BlitSurface(gicle3, NULL, ecran, &position);
              };

               if ( bloodDecap[1] == 4 ){
                  position.x = (xB * 16) + 1, position.y = ((yMB - 2) * 16) - 2;
                  SDL_BlitSurface(gicle1, NULL, ecran, &position);
              };
               if ( bloodDecap[1] == 5 ){
                  position.x = (xB * 16) + 1, position.y = ((yMB - 2) * 16) - 2;
                  SDL_BlitSurface(gicle2, NULL, ecran, &position);
              };
               if ( bloodDecap[1] == 6 ){
                  position.x = (xB * 16) + 1, position.y = ((yMB - 2) * 16) - 2;
                  SDL_BlitSurface(gicle3, NULL, ecran, &position);
              };
          };

     };

     ///                           taches de sang au combat;

      if ( GOTOU == 0 && blood[0] == 0 ){ GOTOU = 15 ; }


       if ( GOTOU == 0 ){
           if ( pA == 69 || pA == 71 ){
              position.x = xA + 2, position.y = positionSANG;
                };

           if ( pA == 78 || pA == 79 ){
              position.x = xA + 2, position.y = positionSANG;
                };

           if ( pA == 73 || pA == 75 ){
              position.x = xA + 2, position.y = positionSANG;
                };

            position.x = (position.x - 1) * 16, position.y = position.y * 16;

            SDL_BlitSurface(tachesang, NULL, ecran, &position);

      };


     //sang2 ;
      if ( GOTOU == 15 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 && blood[1] == 0 ){ GOTOU = 11 ; }


       if ( GOTOU == 0 ){
           if ( pB == 69 || pB == 71 ){
              position.x = xB, position.y = positionSANG;
                };

           if ( pB == 78 || pB == 79 ){
              position.x = xB, position.y = positionSANG;
                };

           if ( pB == 73 || pB == 75 ){
              position.x = xB, position.y = positionSANG;
                };

            position.x = position.x * 16, position.y = position.y * 16;

            SDL_BlitSurface(tachesang, NULL, ecran, &position);

      };



     ///                       ****************************************;
     ///                       ********* affichage du serpent *********;
     ///                       ****************************************;
     //serpent ;
      if ( GOTOU == 11 ){ GOTOU = 0 ; }



      ///                                   selection tete serpent[0];

      ///if ( GOTOU == 0 && serpent[0] == 0 ){ GOTOU = 10 ; }

       if ( GOTOU == 0 && serpent[0] == 1 ){

            position.x = 22, position.y = 44;

           if ( GOTOU == 0 && temps > refserpent[0] + 32 ){
               ////SDL_BlitSurface(serpent2A, NULL, ecran, &position);
               serpent[0] = 0 , GOTOU = 10 ;
          };
           if ( GOTOU == 0 && temps > refserpent[0] + 24 ){
               ////SDL_BlitSurface(serpent3A, NULL, ecran, &position);
               GOTOU = 10 ;
          };
           if ( GOTOU == 0 && temps > refserpent[0] + 16 ){
               ////SDL_BlitSurface(serpent4A, NULL, ecran, &position);
               GOTOU = 10 ;
          };
           if ( GOTOU == 0 && temps > refserpent[0] + 8 ){
               ////SDL_BlitSurface(serpent3A, NULL, ecran, &position);
               GOTOU = 10 ;
          };

           if ( GOTOU == 0 && temps >= refserpent[0] ){
               ////SDL_BlitSurface(serpent2A, NULL, ecran, &position);
               GOTOU = 10 ;
          };


      };


     ///                                   selection tete serpent[1];
     //serpent2 ;
      if ( GOTOU == 10 ){ GOTOU = 0 ; }

      ///if ( GOTOU == 0 && serpent[1] == 0 ){ GOTOU = 17 ; }

      if ( GOTOU == 0 && serpent[1] == 1 ){

            position.x = 550, position.y = 44;

           if ( GOTOU == 0 && temps > refserpent[1] + 32 ){
               ////SDL_BlitSurface(serpent2B, NULL, ecran, &position);
               GOTOU = 17 ;
          };
           if ( GOTOU == 0 && temps > refserpent[1] + 24 ){
               ////SDL_BlitSurface(serpent3B, NULL, ecran, &position);
               serpent[1] = 0 , GOTOU = 17 ;
          };
           if ( GOTOU == 0 && temps > refserpent[1] + 16 ){
               ////SDL_BlitSurface(serpent4B, NULL, ecran, &position);
               GOTOU = 17 ;
          };
           if ( GOTOU == 0 && temps > refserpent[1] + 8 ){
               ////SDL_BlitSurface(serpent3B, NULL, ecran, &position);
               GOTOU = 17 ;
          };

           if ( GOTOU == 0 && temps >= refserpent[1] ){
               ////SDL_BlitSurface(serpent2B, NULL, ecran, &position);
               GOTOU = 17 ;
          };


     };

      ///                       ****************************************;
      ///                       ********* affichage des tetes **********;
      ///                       ****************************************;
      //tete2 ;
       if ( GOTOU == 17 ){ GOTOU = 0 ; }


       if ( GOTOU == 0 && gnome == 1 ){

           if ( GOTOU == 0  ){
			   if ( pA == 78 || pB == 78 || pA == 79 || pB == 79 ){ GOTOU = 14 ; }
		   ; }

           if ( GOTOU == 0 && football == 1 ){ GOTOU = 18 ; }

           if ( GOTOU == 0 && xGNOME < 35 && (xGNOME * 8) > xSPRt - 16 ){ football = 1, reftempsB = temps ; }

           if ( GOTOU == 0 ){ GOTOU = 19 ; }

          //gnometapetete ;
           if ( GOTOU == 18 ){ GOTOU = 0 ; }

           if ( GOTOU == 0 && football == 1 ){

               if ( GOTOU == 0 && temps > reftempsB + 81 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 80 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 5, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 76 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 75 ){ ySPRt = 151, xSPRt = xSPRt + 16, numerotete = 6, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 71 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 70 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 1, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 66 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 65 ){ ySPRt = 157, xSPRt = xSPRt + 16, numerotete = 2, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 61 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 60 ){
                    if ( xGNOME < 33 && xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                  ySPRt = 159, xSPRt = xSPRt + 16, numerotete = 3, GOTOU = 19;
              };
               if ( GOTOU == 0 && temps > reftempsB + 56 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 55 ){ ySPRt = 157, xSPRt = xSPRt + 16, numerotete = 4, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 51 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 50 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 5, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 46 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 45 ){ ySPRt = 151, xSPRt = xSPRt + 16, numerotete = 6, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 41 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 40 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 1, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 36 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 35 ){ ySPRt = 157, xSPRt = xSPRt + 16, numerotete = 2, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 31 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 30 ){
                    if ( xGNOME < 33 && xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                  ySPRt = 159, xSPRt = xSPRt + 16, numerotete = 3, GOTOU = 19;
              };
               if ( GOTOU == 0 && temps > reftempsB + 26 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 25 ){ ySPRt = 157, xSPRt = xSPRt + 16, numerotete = 4, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 21 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 20 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 5, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 16 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 15 ){ ySPRt = 151, xSPRt = xSPRt + 16, numerotete = 6, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 11 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 10 ){ ySPRt = 153, xSPRt = xSPRt + 16, numerotete = 1, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 6 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 5 ){ ySPRt = 157, xSPRt = xSPRt + 16, numerotete = 2, GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 2 ){ GOTOU = 19 ; }
               if ( GOTOU == 0 && temps > reftempsB + 1 ){
                    if ( xGNOME < 33 && xSPRt < 273 ){ PlayAPCM(2, tete, 0) ; }
                  GOTOU = 19;
              };

          };

          //afficheteombre ;
           if ( GOTOU == 19 ){ GOTOU = 0 ; }

           if ( GOTOU == 0 ){
               if ( pA == 80 || pA == 81 ){

                  /// choix du sprite tete decap A;

                   if ( numerotete == 1 ){ SPRtete = tete1A ; }
                   if ( numerotete == 2 ){ SPRtete = tete2A ; }
                   if ( numerotete == 3 ){ SPRtete = tete3A ; }
                   if ( numerotete == 4 ){ SPRtete = tete4A ; }
                   if ( numerotete == 5 ){ SPRtete = tete5A ; }
                   if ( numerotete == 6 ){ SPRtete = tete6A ; }
              };

               if ( pB == 80 || pB == 81 ){

                  /// choix du sprite tete decap B;
                   if ( numerotete == 1 ){ SPRtete = tete1B ; }
                   if ( numerotete == 2 ){ SPRtete = tete2B ; }
                   if ( numerotete == 3 ){ SPRtete = tete3B ; }
                   if ( numerotete == 4 ){ SPRtete = tete4B ; }
                   if ( numerotete == 5 ){ SPRtete = tete5B ; }
                   if ( numerotete == 6 ){ SPRtete = tete6B ; }
              };

          };

          /// affichage des tetes;

           if ( GOTOU == 0 ){
              position.x = xSPRt * 2;

              position.y = 351;
              if ( position.x < 640 ){ SDL_BlitSurface(spriteombre, NULL, ecran, &position) ;}

               position.y = (ySPRt * 2) + 16;
               if ( position.x < 640 ){ SDL_BlitSurface(SPRtete, NULL, ecran, &position) ; }

          };

          GOTOU = 14;

      };

       if ( GOTOU == 0 ){
           if ( pA == 80 || pA == 81 || pB == 80 || pB == 81 ){

               if ( pA == 80 || pA == 81 ){
                   if ( numerotete == 1 ){ SPRtete = tete1A ; }
                   if ( numerotete == 2 ){ SPRtete = tete2A ; }
                   if ( numerotete == 3 ){ SPRtete = tete3A ; }
                   if ( numerotete == 4 ){ SPRtete = tete4A ; }
                   if ( numerotete == 5 ){ SPRtete = tete5A ; }
                   if ( numerotete == 6 ){ SPRtete = tete6A ; }
              };
               if ( pB == 80 || pB == 81 ){
                   if ( numerotete == 1 ){ SPRtete = tete1B ; }
                   if ( numerotete == 2 ){ SPRtete = tete2B ; }
                   if ( numerotete == 3 ){ SPRtete = tete3B ; }
                   if ( numerotete == 4 ){ SPRtete = tete4B ; }
                   if ( numerotete == 5 ){ SPRtete = tete5B ; }
                   if ( numerotete == 6 ){ SPRtete = tete6B ; }
              };

              /// affichage des tetes;

               if ( GOTOU == 0 ){
                  xSPRt = (xSPRt - 1) * 8; // laisser en 8 pour le football ,);

                  position.x = xSPRt * 2;

                   position.y = 351;
                   if ( position.x < 640 ){ SDL_BlitSurface(spriteombre, NULL, ecran, &position) ;}

                   position.y = (ySPRt * 2) + 16;
                   if ( position.x < 640 ){ SDL_BlitSurface(SPRtete, NULL, ecran, &position) ; }

              };

          };


      };



    ///                       ****************************************;
    ///                       ********* affichage du gnome  **********;
    ///                       ****************************************;
    //gnome2 ;
     if ( GOTOU == 14 ){ GOTOU = 0 ; }

      if ( GOTOU == 0 && gnome == 1 ){

          /// choix du sprite;
           if ( numerognome == 1 ){ SPRgnome = gnome1 ; }
           if ( numerognome == 2 ){ SPRgnome = gnome2 ; }
           if ( numerognome == 3 ){ SPRgnome = gnome3 ; }
           if ( numerognome == 4 ){ SPRgnome = gnome4 ; }

          /// affichage du gnome;
          //if ( xGNOME < 40 ){
                position.x = ((xGNOME - 2) * 16);
                position.y = 303;
                SDL_BlitSurface(SPRgnome, NULL, ecran, &position) ;
            //};


           if ( xGNOME > 39 ){

               if ( pB == 80 || pB == 81 ){
                   if ( partie == 2 ){ bonus[0] = 1 ; }
                   if ( partie == 1 ){
                       if ( xB >= 39 ){ sortieA = 1, Aoccupe = 0 ; }
                  };
              };
               if ( pB == 78 || pB == 79 ){
                   if ( partie == 2 ){ bonus[0] = 1 ; }
                   if ( partie == 1 ){
                       if ( xB >= 39 ){ sortieA = 1, Aoccupe = 0 ; }
                  };
              };

               if ( pA == 80 || pA == 81 ){
                   if ( partie == 2 ){ bonus[1] = 1 ; }
                   if ( partie == 1 ){
                       if ( xA >= 39 ){ sortieB = 1, Boccupe = 0 ; }
                  };
              };
               if ( pA == 78 || pA == 79 ){
                   if ( partie == 2 ){ bonus[1] = 1 ; }
                   if ( partie == 1 ){
                       if ( xA >= 39 ){ sortieB = 1, Boccupe = 0 ; }
                  };
              };
          };


          /// cadavre tire par le gnome;

           if ( pA == 80 || pA == 78 ){
               if ( xGNOME > xA + 5 ){
                  gnometire = 0;
                  xA = xA + 1;
              };
          };
           if ( pA == 81 || pA == 79 ){
               if ( xGNOME > xA + 4 ){
                  gnometire = 0;
                  xA = xA + 1;
              };
          };

           if ( pB == 80 || pB == 78 ){
               if ( xGNOME > xB + 4 ){
                  gnometire = 0;
                  xB = xB + 1;
              };
          };
           if ( pB == 81 || pB == 79 ){
               if ( xGNOME > xB + 4 ){
                  gnometire = 0;
                  xB = xB + 1;
              };
          };

     };


     ///                       ****************************************;
     ///                       ************ affichage du feu **********;
     ///                       ****************************************;
     //boulefeu ;
      if ( GOTOU == 9 ){ GOTOU = 0 ; }

       if ( GOTOU == 0 && jeu == 1 && sorceror == 1 && xAttB > xB + 4 ){

          position.x = (xAttB - 2) * 16, position.y = yAttB * 16;

           if ( xAttB == 9 || xAttB == 18 || xAttB == 27 ){
              SPRfeu = feu1;
              SDL_BlitSurface(SPRfeu, NULL, ecran, &position);
          };

           if ( xAttB == 12 || xAttB == 21 ){
              SPRfeu = feu2;
              SDL_BlitSurface(SPRfeu, NULL, ecran, &position);;
          };

           if ( xAttB == 15 || xAttB == 24 ){
              SPRfeu = feu3;
              SDL_BlitSurface(SPRfeu, NULL, ecran, &position);
          };


      };



     ///                       ****************************************;
     ///                       ********* affichage des arbres**********;
     ///                       ****************************************;
      if ( GOTOU == 0 ){

          if ( xA <= 6 || xB <= 6 ){
                position.x = 0;
                position.y = 208;
                if ( decorS == 1 ){ SDL_BlitSurface(arbreG1, NULL, ecran, &position); }
                if ( decorS == 2 ){ SDL_BlitSurface(arbreG2, NULL, ecran, &position); }
                if ( decorS == 3 ){ SDL_BlitSurface(arbreG3, NULL, ecran, &position); }
                if ( decorS == 4 ){ SDL_BlitSurface(arbreG4, NULL, ecran, &position); }
        };

          if ( gnome == 1 && xGNOME <= 12 ){
                position.x = 0;
                position.y = 208;
                 if ( decorS == 1 ){ SDL_BlitSurface(arbreG1, NULL, ecran, &position); }
                if ( decorS == 2 ){ SDL_BlitSurface(arbreG2, NULL, ecran, &position); }
                if ( decorS == 3 ){ SDL_BlitSurface(arbreG3, NULL, ecran, &position); }
                if ( decorS == 4 ){ SDL_BlitSurface(arbreG4, NULL, ecran, &position); }
         };

          if ( xB >= 29 || xA >= 29 || xGNOME >= 32 || xSPRt*2 >= 526 ){
                position.x = 544;
                position.y = 208;
                if ( decorS == 1 ){ SDL_BlitSurface(arbreD1, NULL, ecran, &position); }
                if ( decorS == 2 ){ SDL_BlitSurface(arbreD2, NULL, ecran, &position); }
                if ( decorS == 3 ){ SDL_BlitSurface(arbreD3, NULL, ecran, &position); }
                if ( decorS == 4 ){ SDL_BlitSurface(arbreD4, NULL, ecran, &position); }
         };

     };

     ///                       ****************************************;
     ///                       ********* jeu gagne - jeu perdu ********;
     ///                       ****************************************;


      	if ( GOTOU == 0 && version == 1 ){ // easter egg :)
                    texteDestination.x = 304;
                    texteDestination.y = 288;
                    SDL_Surface *easteregg1 = TTF_RenderText_Solid(police,"FL",fontColor2) ;
                    SDL_BlitSurface(easteregg1,NULL,ecran,&texteDestination);
                    texteDestination.x = 288;
                    texteDestination.y = 308;
                    SDL_Surface *easteregg2 = TTF_RenderText_Solid(police,"2016",fontColor2) ;
                    SDL_BlitSurface(easteregg2,NULL,ecran,&texteDestination);
      	};

       if ( GOTOU == 0 ){

           if ( jeu == 2 ){

                    cacheBLEU = SDL_CreateRGBSurface(SDL_HWSURFACE, 240, 16, 32, 0, 0, 0, 0);
                    SDL_FillRect(cacheBLEU, NULL, SDL_MapRGB(ecran->format, 34, 34, 153));
                    position.x = 192;
                    position.y = 144;
                    SDL_BlitSurface(cacheBLEU, NULL, ecran, &position);

                    texteDestination.x = 192;
                    texteDestination.y = 144;
                    SDL_Surface *messageFIN = TTF_RenderText_Solid(police,"Thanks big boy.",fontColor) ;
                    SDL_BlitSurface(messageFIN,NULL,ecran,&texteDestination);

          };

           if ( jeu == 0 && pA == 84 ){ /// 84

                    cacheBLEU = SDL_CreateRGBSurface(SDL_HWSURFACE, 288, 16, 32, 0, 0, 0, 0);
                    SDL_FillRect(cacheBLEU, NULL, SDL_MapRGB(ecran->format, 34, 34, 153));
                    position.x = 160;
                    position.y = 144;
                    SDL_BlitSurface(cacheBLEU, NULL, ecran, &position);

                    texteDestination.x = 160;
                    texteDestination.y = 144;
                    SDL_Surface *messageFIN = TTF_RenderText_Solid(police,"Your end has come!",fontColor) ;
                    SDL_BlitSurface(messageFIN,NULL,ecran,&texteDestination);


          };

      };




     ///                        gestion du temps et retour;



      if ( arret == 1 ){




          if ( jeu == 1 && temps > 30 ){


            //halt all sounds
			score[0] = 0;
			score[1] = 0;

               if ( GOTOU == 0 && demo == 1 ){
                  partie = 1, decorS = 2, IA = 0, sorceror = 0, marianna = 0, demo = 0;
                  GOTOU = 8;
              };

               if ( GOTOU == 0 && partie == 1 ){
                  partie = 2, decorS = 2, IA = 0, sorceror = 0, marianna = 0, timer = 60;
                  GOTOU = 8;
              };

               if ( GOTOU == 0 && partie == 2 ){
                  partie = 1, decorS = 2, IA = 4, sorceror = 0, marianna = 0, demo = 1;
                  GOTOU = 8;
              };

         };

           if ( jeu == 2 || jeu == 0 ){
		score[0] = 0;
		score[1] = 0;
              partie = 1, decorS = 2, IA = 0, sorceror = 0, marianna = 0;
              GOTOU = 8;
          };


     };



		goto MAIN_TOP;

		}
	}
}
