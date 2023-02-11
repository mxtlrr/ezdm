#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"
#include <stdio.h>

void _(){ return; }

int main(){
  SetTraceLogCallback(_);
  InitWindow(GetScreenWidth(), GetScreenHeight(), "Easy Display Manager");

  char username_text[256], password_text[256] = "";
  bool username_editmode, password_editmode = false;


  while(!WindowShouldClose()){
    BeginDrawing();

      // set color
      ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));

      DrawText("Username: ", 100, 200, 40, BLACK);
      DrawText("Password: ", 100, 300, 40, BLACK);

      // X,Y,W,H
      Rectangle username_bounds = { 320, 200, 255, 40 };
      if(GuiTextBox(username_bounds, username_text, 40, username_editmode))
        username_editmode = !username_editmode;
      

      Rectangle password_bounds = { 320, 300, 255, 40 };
      if(GuiTextBox(password_bounds, password_text, 40, password_editmode))
        password_editmode = !password_editmode;

      
      Rectangle button_bounds = { 390, 350, 105, 40};
      if(GuiButton(button_bounds, "Login") == true){
        printf("got username as: %s and password: %s\n", username_text, password_text);
      }
      

    EndDrawing();
  }
}