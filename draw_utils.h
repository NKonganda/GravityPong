#pragma once

void renderText(const char* text, int x, int y, TTF_Font* font, SDL_Renderer* renderer, unsigned char r = 0, unsigned char g = 0, unsigned char  b = 0, float scale = 1) {    
        SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, text, {r,g,b,0}); 
        SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        int w, h;
        TTF_SizeText(font, text, &w, &h);

        SDL_Rect Message_rect; 
        Message_rect.x = x;
        Message_rect.y = y; 
        Message_rect.w = w * scale;
        Message_rect.h = h * scale; 

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
}


int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
};

bool renderButton(SDL_Renderer* renderer, const char* text, TTF_Font* font, int x, int y, int mouseX, int mouseY) {
    int w, h;
    TTF_SizeText(font, text, &w, &h);
    SDL_Rect rect = {x,y,w,h};
    if (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h) {
        SDL_SetRenderDrawColor(renderer, 255,255,255,150);
        if (SDL_GetMouseState(0,0) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
            return true;
        }
    }
    else {
        SDL_SetRenderDrawColor(renderer, 150,150,150,150);
    }
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0,0,0,0);
    SDL_RenderDrawRect(renderer, &rect);
    renderText(text, x, y, font, renderer, 1);
    return false;
}




std::string textInput;
std::string renderTextInput(SDL_Renderer* renderer, TTF_Font* font, int x, int y, int limit = INT_MAX, float scale = 1) {

    if(textInput.length() > 0 && textInput.at(textInput.length() - 1) == '\n') {
        std::string value = textInput;
        textInput = "";
        return value;
    }

    if(textInput.length() > limit) {
        textInput = textInput.substr(0, limit);
    }

    int h = 50;
    int w = limit * 30;
    float wf = w;
    float hf = h;
    SDL_Rect rect = {x, y, int(wf * scale), int(hf * scale)};
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer,100,100,100,255);
    SDL_RenderDrawRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    renderText(textInput.c_str(), x,y,font,renderer,255,255,255, scale * 2);

    SDL_SetRenderDrawColor(renderer,255,255,255,255);
    int ww, hh;
    TTF_SizeText(font, textInput.c_str(), &ww, &hh);
    ww *= scale;
    SDL_Rect rect2 = {x + (ww * 2), y + 2, 5,(h-4)};
    SDL_RenderFillRect(renderer, &rect2);

    return "";
}
