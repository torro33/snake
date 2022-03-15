#include "graphics.hpp"
#include <cstdlib>
#include <vector>
#include <iostream>
using namespace genv;
using namespace std;
const int X = 600;//600/30=20   20*20 as pálya
const int Y = 600;
enum Direction {STOP = 0,LEFT,RIGHT,UP,DOWN };

struct food{//private mezõk
    private:
        int x;
        int y;
    public:
        void setX(int a){
           x = a;
        }
        int getX(){
            return x;
        }
        void setY(int b){
           y = b;
        }
        int getY(){
            return y;
        }

    food(int a,int b){
        x = a;
        y = b;
    }
};//food struct

struct snakebody{
    int x;
    int y;
    public : snakebody(int a,int b){
       x = a;
       y = b;
    }
};//snakebody struct

struct snake{
        vector <snakebody> Vector;
        public : snake(){
        Vector.push_back(snakebody(10,11));//oszlop,sor
        Vector.push_back(snakebody(10,12));
        Vector.push_back(snakebody(10,13));
        Vector.push_back(snakebody(10,14));
    }
};//snake struct

struct simulation{
    bool gameover = false;
    Direction dir = UP;
    void draw(vector <snakebody> &Vector,food f,snakebody head){
        gout << color(0,0,0) << move_to(0,0) << box(X,Y);//tiszta kép
                gout << color(0,0,255) << move_to(0,0) << box(X,30);//4 fal kirajzol
                gout << color(0,0,255) << move_to(0,0) << box(30,Y);
                gout << color(0,0,255) << move_to(X-30,0) << box(30,Y);
                gout << color(0,0,255) << move_to(0,Y-30) << box(X,30);
                gout << color(0,255,0) << move_to(head.x*30,head.y*30) << box(30,30);//fej kirajzol
        for(int i = 0;i<Vector.size();i++){//kirajzolja a kígyót
            gout << color(0,0,255) << move_to(Vector[i].x*30,Vector[i].y*30) << box(30,30);
        }
        gout << color(255,0,0) << move_to(f.getX()*30,f.getY()*30) << box(30,30);//kaja kirajzol
        gout << refresh;
    }

    void input(event ev){//billentyût beolvas, irányt beállít
            if(ev.keycode == key_left){
                dir = LEFT;
            }else if(ev.keycode == key_right){
                dir = RIGHT;
            }else if(ev.keycode == key_up){
                dir = UP;
            }else if(ev.keycode == key_down){
                dir = DOWN;
            }
    }

    void wiggle(vector <snakebody> &Vector,food &f,snakebody &head){
        snakebody help(Vector[0]);//mindegyik kígyórész felveszi az elõzõ koordinátáit
        snakebody help2(0,0);
        Vector[0] = head;//a nulladik elem a fej koordinátáit veszi át
        for(int i = 1;i<Vector.size();i++){
           help2 = Vector[i];
           Vector[i] = help;
           help=help2;
        }
        switch(dir){//lépteti a fejet a megfelelõ irányba
		case 1:
			head.x--;
			break;
		case 2:
			head.x++;
			break;
		case 3:
			head.y--;
			break;
		case 4:
			head.y++;
			break;
		default:
			break;
		}
        if(head.x > X/30-2 || head.x < 1 || head.y > Y/30-2 || head.y < 1){//ha nekimegyünk a falnak akkor vége
        nyekk();
        }
        for(int i = 0;i<Vector.size();i++){//magába megy meghal
            if(Vector[i].x == head.x && Vector[i].y == head.y){
                nyekk();
            }
        }
            if(head.x == f.getX() && head.y == f.getY()){//ha a kígyó fejének koordinátái megegyeznek a kaja koordinátájával akkor újat generál
                bool separatefood = true;
                bool check = true;
                while(separatefood){
                    f.setX(rand()%(X/30-2));
                    f.setY(rand()%(Y/30-2));
                    f.setX(f.getX()+1);
                    f.setY(f.getY()+1);
                    for(int i = 0;i<Vector.size();i++){//ne hozzon létre kaját a kígyóra rá
                        if(f.getX() == Vector[i].x && f.getY() == Vector[i].y || f.getX() == head.x && f.getY() == head.y){
                            check = false;
                        }
                    }
                        if(check){
                            Vector.push_back(snakebody(0,0));//vagy X,Y
                            separatefood = false;
                        }
                }
            }
        }//wiggle() end

        void nyekk(){//gameover képernyõ kirajzolás
            gameover = true;
            dir = STOP;
            gout << move_to(0,0) << color(0,0,255) <<box(X,Y);
            gout << move_to(50,350) << color(0,0,0) << line_to(50,100) << line_to(100,350) << line_to(100,100);//N
            gout << move_to(150,350) << color(0,0,0) << line_to(200,100) << move_to(175,220) << line_to(150,100);//Y
            gout << move_to(300,100) << color(0,0,0) << line_to(250,100) << line_to(250,225) << line_to(300,225) << move_to(250,225) << line_to(250,350) << line_to(300,350);//E
            gout << move_to(350,100) << color(0,0,0) << line_to(350,225) << line_to(400,100) << move_to(350,225) << line_to(350,350) << move_to(350,225) << line_to(400,350);//K
            gout << move_to(450,100) << color(0,0,0) << line_to(450,225) << line_to(500,100) << move_to(450,225) << line_to(450,350) << move_to(450,225) << line_to(500,350);//K
            gout << move_to(250,550) << color(0,0,0) <<text("PRESS ESC TO EXIT");
        }

        void menu(){//kezdõképernyõ
            gout << move_to(0,0) << color(0,0,255) <<box(X,Y);
            gout << move_to(258,50) << color(0,0,0) << text("SNAKE GAME");
            gout << move_to(225,270) << color(0,0,0) <<text("PRESS ENTERT TO START") << refresh;
        }

};//simulation struct

int main()
{
    gout.open(X,Y);
    snake s;
    food f(10,8);
    snakebody head(10,10);
    simulation si;
    event ev;
    gin.timer(200);//200 ideális
    while(gin >> ev && ev.keycode != key_enter){//kezdõmenü
    si.menu();
    }
    while(gin >> ev && ev.keycode != key_escape){
            if(!si.gameover){//addig fut amíg nem ütközik
                si.draw(s.Vector,f,head);
                if(ev.type == ev_key){
                si.input(ev);
                }
                else if(ev.type == ev_timer){
                si.wiggle(s.Vector,f,head);
                }
                        gout << refresh;
            }
    }
    return 0;
}
