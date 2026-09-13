#include <glc2d.h>
#include <cassert>
#include <string>
#include <vector>
#include <iostream>
#define private public
#include "GameUI.h"
#undef private
#include "ScenceGamePlay.h"
void GameUI::PlaySelect() const {}
std::vector<std::string> shown;
int g2_TextureLoad(CSTR, DWORD) { return 1; }
int g2_TextureRelease(int) { return 0; }
bool GameUI::Hit(const RECT&) const { return false; }
void GameUI::Image(int,float,float,float,float,DWORD) const {}
void GameUI::Panel(const RECT&,DWORD) const {}
void GameUI::Text(const char* t,const RECT&,int,DWORD) const { shown.emplace_back(t); }
void GameUI::Button(const char* t,const RECT&) const { shown.emplace_back(t); }
bool has(const std::string& text) { for (auto& s:shown) if(s.find(text)!=std::string::npos) return true; return false; }
int main() {
 GameUI ui; ScenceGamePlay scene; Player player;
 auto press=[&](int key) { for(bool& b:ui.m_pressed)b=false; ui.m_pressed[key]=true; return scene.Update(ui,player); };
 auto render=[&]() {shown.clear();scene.Render(ui,player);};
 for(int choice=1;choice<=2;++choice) {
  player.Init();scene.Reset();render();assert(has("사흘째 같은 시간"));
  assert(player.GetHP()==100&&player.GetLife()==3&&player.GetScore()==0);
  for(int i=0;i<26;++i)assert(!press(VK_RETURN));
  render();assert(has("1. 서연의 말을")&&has("2. 증거를"));
  for(int i=0;i<3;++i)assert(!press(VK_RETURN));
  render();assert(has("1. 서연의 말을"));
  assert(!press(choice==1?'1':'2'));
  assert(player.GetScore()==(choice==1?10:0));
  for(int i=0;i<5;++i)assert(!press(choice==1?'1':'2'));
  assert(player.GetScore()==(choice==1?10:0));
  render();assert(has(choice==1?"우선 네 말을 믿고":"판단은 증거를"));
  assert(!press(VK_SPACE));render();assert(has(choice==1?"끝까지 같이":"내 기억이 무조건"));
  assert(!press(VK_SPACE));render();assert(has("첫 만남 완료"));
  assert(press(VK_RETURN));
 }
 scene.Reset();assert(press(VK_ESCAPE));
 std::cout<<"PASS: 26 lines, choice gate, both responses, single score award, completion, reset, Escape\n";
}
