#!/usr/bin/env python3
import kivy
import rospy
from kivy.app import App
from kivy.uix.floatlayout import FloatLayout
from kivy.core.window import Window
from kivy.uix.button import Button
from kivy.uix.image import Image
from std_msgs.msg import Int16
from kivy.core.window import Window
Window.size=(1280,800)

class FondApp(App):
 
    def build(self):

        self.Fond=FloatLayout()
        self.Img1=Image(source='Zmap.jpg',allow_stretch=0,keep_ratio=133)
        self.Fond.pos_hint = {"center_x": 0.5, "center_y":0.5}
        self.button1=Button(text='',size_hint=(212/1280,(526-460)/800),pos=(320,800-526),background_color =(0,0,0,0)) 
        self.button1.bind(on_press=self.button_function1)
        self.Fond.add_widget(self.Img1)
        self.Fond.add_widget(self.button1)
        self.button2=Button(text='',size_hint=(250/1280,(550-460)/800),pos=(320,564),background_color =(0,0,0,0))
        self.button2.bind(on_press=self.button_function2)
        self.Fond.add_widget(self.button2)
        self.button3=Button(text='',size_hint=(250/1280,(550-460)/800),pos=(575,564),background_color =(0,0,0,0))
        self.button3.bind(on_press=self.button_function3)
        self.Fond.add_widget(self.button3)
        self.button4=Button(text='',size_hint=(250/1280,(550-460)/800),pos=(830,564),background_color =(0,0,0,0))
        self.button4.bind(on_press=self.button_function4)
        self.Fond.add_widget(self.button4)
        self.button5=Button(text='',size_hint=(0.075,0.47),pos=(220,800-526),background_color =(0,0,0,0))
        self.button5.bind(on_press=self.button_function5)
        self.Fond.add_widget(self.button5)
        self.button6=Button(text='',size_hint=(0.098,0.44),pos=(960,800-590),background_color =(0,0,0,0))
        self.button6.bind(on_press=self.button_function6)
        self.Fond.add_widget(self.button6)
        self.button7=Button(text='',size_hint=(200/1280,(590-460)/800),pos=(765,800-590),background_color =(0,0,0,0))
        self.button7.bind(on_press=self.button_function7)
        self.Fond.add_widget(self.button7)
        #To make transparent background for buttons use : background_color =(0,0,0,0)

        return self.Fond
    def button_function5(self,instance):
        print("Zara")
        pub.publish(0)
    def button_function1(self,instance):
        print("H&M")
        pub.publish(1)
    def button_function2(self,instance):
        print("adidas")
        pub.publish(2)
    def button_function3(self,instance):
        print("US Polo")
        pub.publish(3)
    def button_function4(self,instance):
        print("baskin robbins")
        pub.publish(4)

    def button_function6(self,instance):
        print("vox")
        pub.publish(5)
    def button_function7(self,instance):
        print("vox")
        pub.publish(5)

if __name__ == '__main__':
    rospy.init_node('gui', anonymous=True)
    pub = rospy.Publisher('gui_goal', Int16, queue_size=10)
    FondApp().run()
