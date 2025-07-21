# nice!beetle Fork

![image](images/beedly_animation_black.gif)![image](images/beedly_animation_orange.gif)![image](images/beedly_animation_grey.gif)![image](images/beedly_animation_white.gif)![image](images/beedly_animation_black.gif)![image](images/beedly_animation_orange.gif)![image](images/beedly_animation_grey.gif)![image](images/beedly_animation_white.gif)

This is a forked repo of mctechnology17's fantastic [zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled) repo. Please give it a look and some love! None of this would be possible without me plagiarising his code.

This fork changes the gem animation of the nice!oled and nice!epaper to be a Tamagotchi widget of a beetle named Beedly. Why? Because I like beetles and the gem animation wasn't doing it for me.

![image](images/beedly_peripheral.gif)

This fork also provides a guide and (some) tools necessary to display your own images an animation.

See [TODO.md](TODO.md) for planned/future improvements.

# Changing the peripheral animation

The main bulk of code works by displaying CF indexed 1 bit files defined in `crystal.c` and played back using `animation.c`. 

This means, once we have an image, we can convert it, change the code and then display our image.

Firstly, create an animation or static image to display. I used [Aseprite](https://www.aseprite.org/), as this is what I've most been used to in the past. The image will only fit on the peripheral if it's at most 160x68 pixels ([nice!view documentation](https://nicekeyboards.com/nice-view/)).

Beedly plus the UI is a healthy (but not too large) 68x115. Take size constraints into mind when picking or making an image.

Once you have your image, rotate your it 90 degrees clockwise. The Nice!View display is actually a horizontal display that works vertically, as such we need to account for this. Export your animation frames or still image in `.png` format. 

Then, translate your images into a LBGL C array using the [LVGL image converter](https://lvgl.io/tools/imageconverter). Select `LVGL v8` and `CF_INDEXED_1_BIT` colour format with C array output. 

Once you've got your files, copy the map data and override the frames in `crystal.c`. The IFNDEF block at the top of the data allows the image to be inverted, which is something I need to improve. 

Make sure your main zmk `west.yaml` file points to this repo and the corresponding branch and you're good to go. 
