# nice!beetle Fork

This is a forked repo of mctechnology17's fantastic [zmk-nice-oled](https://github.com/mctechnology17/zmk-nice-oled) repo. Please give it a look and some love! 

This fork changes the gem animation to be a crude little beetle because they're my favourite animal. 

This will act as a sort of guide to myself and maybe others as to how to change the right hand peripheral animation.

# Changing the peripheral animation
This can be done by changing the frames defined in `crystal.c` and `animation.c`. 

First, create an animation or static image to display. I used [Aseprite](https://www.aseprite.org/), as this is what I've most been used to in the past. However, you can use other free options out there. 

Second, rotate your canvas 90 degrees clockwise. The Nice!View display is actually a horizontal display that works nicely vertically, as such we need to account for this. Export your animation frames or still image in `.png` format. 

Third, translate your images into a LBGL C array using the [LVGL image converter](https://lvgl.io/tools/imageconverter). Select `LVGL v8` and `CF_INDEXED_1_BIT` colour format with C array output. 

Once you've got your files, copy the map data and override the frames in `crystal.c`. The IFNDEF block at the top of the data allows the image to be inverted, which is something I need to improve. 

Make sure your main zmk `west.yaml` file points to this repo and the corresponding branch and you're good to go. 
