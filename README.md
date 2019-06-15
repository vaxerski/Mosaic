[![C++ver](http://vaxerski.xyz/cppbadge.svg)](http://vaxerski.xyz/cppbadge.svg)
[![stability](http://vaxerski.xyz/stability.svg)](http://vaxerski.xyz/stability.svg)
[![optimization](http://vaxerski.xyz/optimization.svg)](http://vaxerski.xyz/optimization.svg)
[![HitCount](http://hits.dwyl.io/vaxerski/Mosaic.svg)](http://hits.dwyl.io/vaxerski/Mosaic)

# Mosaic
A handy application for making mosaic out of images<br/>
!NOTE: Mosaic starting from v1.5 is 64-bit only. You need to compile the app for 32-bit by yourself if you have a 32-bit system somehow.

## Usage
Put any composite `.png`'s in the /images/ folder. It needs to be **directly in the same folder as the app.**<br/>
Put one source.png in the folder **with the app**<br/>

Run the app.

Edit the config if anything's wrong.

Press `F11` to make an image of the mosaic and save it to /screenshots/. Don't worry, even if the viewport is bigger than your screen, F11 **WILL** work.<br/>
Press `ESC` to close the app when finished.

## Config
`max_clones` - Sets max amount of composites on image.<br/>
`precision_point` - Sets amount of precision, here the size of one image. Smaller = more precise.<br/>
`debug_info` - debug info stats

## Future Plans
- [ ] Make a UI
- [ ] Support for JPEG and different resolutions
- [ ] Easier navigation and usage
- [ ] Do something to the renderer to stop rendering more frames than it has to
- [ ] Make the code less spaghetti
- [ ] *sigh* optimization


## FAQ
**Is this a virus?**

No, it's literally open source. Go check it yourself.<br/><br/>


**Will this slow my computer down?**

Yes, it's doing some quik mafs. After you exit it won't anymore, I guess.<br/><br/>


**Can i use it for my own?**

Yes, see the license.
