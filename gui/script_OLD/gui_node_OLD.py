#!/usr/bin/env python3
import kivy
from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.button import Button
from kivy.uix.image import Image
import rospy
from std_msgs.msg import Int16
from kivy.core.window import Window
Window.size=(1280,800)

class FondApp(App):
 
    def build(self):
        #On cree un Layout racine:
        self.Fond=FloatLayout()
        #On importe des images pour le fond avec des options d'etirement
        self.Img1=Image(source='Map-s1.png',allow_stretch=0,keep_ratio=133)
        self.Fond.pos_hint = {"center_x": 0.5, "center_y":0.5}
        #On cree un bouton:
        self.Bouton1=Button(text='',size_hint=(0.1,0.1),pos=(800,340),background_normal ='mcd1r.png')
        #background_normal ='youtube.ico'
    
        self.Bouton1.bind(on_press=self.callback_bouton1)
        self.Fond.add_widget(self.Img1)
        self.Fond.add_widget(self.Bouton1)
        self.Bouton2=Button(text='',size_hint=(0.1,0.1),pos =(370, 340),background_normal ='zara-1.jpg')
        self.Bouton2.bind(on_press=self.callback_bouton2)
        self.Fond.add_widget(self.Bouton2)
        self.Bouton3=Button(text='',size_hint=(0.1,0.1),pos=(370,590),background_normal ='stard.jpg')
        self.Bouton3.bind(on_press=self.callback_bouton3)
        self.Fond.add_widget(self.Bouton3)
        self.Bouton4=Button(text='',size_hint=(0.1,0.1),pos=(800,600),background_normal ='mu.png')
        self.Bouton4.bind(on_press=self.callback_bouton4)
        self.Fond.add_widget(self.Bouton4)
       
        #On affiche la racine:
        return self.Fond
 
    def callback_bouton1(self,instance):#Pour le bouton
        print("coucou")
        pub.publish(15)
    def callback_bouton2(self,instance):#Pour le bouton
        print("coucou2")
        pub.publish(16)
    def callback_bouton3(self,instance):#Pour le bouton
        print("coucou3")
        pub.publish(17)
    def callback_bouton4(self,instance):#Pour le bouton
        print("coucou4")
        pub.publish(18)
 
if __name__ == '__main__':
    rospy.init_node('gui', anonymous=True)
    pub = rospy.Publisher('gui_goal', Int16, queue_size=10)
    FondApp().run()
