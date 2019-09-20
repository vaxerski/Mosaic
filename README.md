[![C++ver](http://vaxerski.xyz/cppbadge.svg)](http://vaxerski.xyz/cppbadge.svg)
[![stability](http://vaxerski.xyz/stability.svg)](http://vaxerski.xyz/stability.svg)
[![optimization](http://vaxerski.xyz/optimization.svg)](http://vaxerski.xyz/optimization.svg)
[![HitCount](http://hits.dwyl.io/vaxerski/Mosaic.svg)](http://hits.dwyl.io/vaxerski/Mosaic)

# Mosaic
A handy application for making mosaics out of images<br/>
!NOTE: Mosaic starting from v1.5 is 64-bit only. You need to compile the app for 32-bit by yourself if you have a 32-bit system somehow.

## Usage
Precision point - the lower, the more detailed the image will be. (extremely low prec point will slow your pc down and require lots of time)

Max clones - the maximum amount of clones to produce an image. (lower precision points require more clones, more clones = more RAM)

Clone aspect ratio - controls the aspect ratio of individual construction blocks. (I recommend 1:1 for best results)


Composite images must be PNG/JPG/JPEG's and be put in one folder.
Source image can be PNG/JPG/JPEG.

!NOTE: the app may use up to 8GB of RAM while generating if dealing with low precision points on big images, e.g. 4K.

## Suggestions and Bugs

If you find any bugs, or have suggestions, feel free to open an "issue" in the issues tab. Thank you! :)

## Future Plans
- [x] Make a UI
- [x] Support for JPEG and different resolutions - Done fully this time
- [x] Easier navigation and usage
- [x] Do something to the renderer to stop rendering more frames than it has to
- [ ] Make the code less spaghetti
- [ ] *sigh* optimization


## FAQ
**Is this a virus?**

No, it's literally open source. Go check it yourself.<br/><br/>


**Will this slow my computer down?**

Yes, it's doing some quik mafs. But only when it's generating, i guess.<br/><br/>


**Can i use it for my own?**

Yes, see the license.
