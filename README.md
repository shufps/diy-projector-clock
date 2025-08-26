# DIY Night Projector Clock

A Clock Projector can project the time to a ceiling in the night.

<img width="1200" height="797" alt="image" src="https://github.com/user-attachments/assets/4e10f9ec-fc6c-4fd0-b92c-7d45c06cef7d" />

There are lots of commercial alarm clocks with integrated clock projector on the market but I thought, it would be fun to build one myself.

Moreover it was a good project for becoming accustomed to the process of 3d-designing cases which can be 3d printed and to learn about designing tolerances.

The principle is easy – basically it works like a video beamer (or any other projecting clock^^).

<img width="394" height="239" alt="image" src="https://github.com/user-attachments/assets/61ef5b66-bb2f-47a1-8b9c-5fc8be717c69" />

## LCD and Calculations
I choosed a small negative (background black) 64×32 LCD which has the dimensions of just 15mm x 12mm (active area 11.18mm x 5.58mm).

<img width="398" height="347" alt="image" src="https://github.com/user-attachments/assets/a2f8d018-b72a-43ed-ac4b-81ca54046ee0" />

The goal was to build a complete projector with not more than 50mm x 50mm x 50mm (which I didn’t managed … But close to^^).

A small LCD has the advantage that everything becomes smaller and cheaper – this influences heavily costs for optics because a smaller LCD needs smaller optics to project an low distorted image.

Moreover a smaller LCD needs smaller focal length for a decent sized imaged on the ceiling. A good distance would be 2m from LCD to ceiling.

The formula for image width depending from the focal length is defined as:

<img width="600" height="286" alt="image" src="https://github.com/user-attachments/assets/9b33e779-1af5-4aef-b819-b59cf739eb22" />

<img width="362" height="56" alt="image" src="https://github.com/user-attachments/assets/81d37652-1072-4271-9440-f77fb175638a" />

with:
– S2 distance from lens to LCD and
– S1 distance from lens to ceiling
– W2 is the width of the LCD
– W1 is the width of the projected image

The formulas show that the smaller the focal length the larger the projected image will be.

I decided to go for 25mm because there are cheap lenses which can be obtained for about $12 in china.

For our values that would magnify the image by about x80 – 11.1mm LCD width would become 88.1cm image width on the ceiling in a distance of 2m.

## Light and Condensor Lens
It was not really clear how strong a LED has to be in order to get a reasonable well readable picture in the night on the ceiling.

To be sure – actually I wasn’t really – I decided to use a high power LED with 1W and use a cheap chinese condensor lense to parallelize the light before it passes the LCD (this has do be done anyway but a condensor lens makes it possible to use more light because condensors are shaped in a way that allowes to get the lens as near to the LED as possible – so catching more light).

<img width="310" height="283" alt="image" src="https://github.com/user-attachments/assets/5a78adf3-33b4-472e-8988-a4a512a55498" />
<img width="557" height="356" alt="image" src="https://github.com/user-attachments/assets/8494496c-a928-4063-901b-a8a1f96c119c" />

The two pictures show the condensor-lense which is directly mounted to the PCB the resulting light spot on the ceiling. It is a really gread lens which is made for my LED – so it fits perfectly and parallelizes the light nicely.

## Electronics Construction
Two PCBs were designed – one holding the LCD and the actual projecting board.

<img width="455" height="414" alt="image" src="https://github.com/user-attachments/assets/897fcd8a-6bf1-47b8-a4fe-6b4d9519bd98" />
<img width="537" height="414" alt="image" src="https://github.com/user-attachments/assets/09c0e21f-239f-4577-bec9-b8f0bae4cc39" />

The latter is equipped with 1W LED driver, STM32F103 Cortex ARM, a rotary encoder (for changing settings like brightness, standby-timer, time, …), USB, IR (both not used yet).

## Mechanics Construction
Originally, I aimed for 50mm x 50mm x 50mm but I only managed to get to 60mm x 60mm x 60mm 😥 😉

Here an overview about all I designed:

<img width="1028" height="647" alt="image" src="https://github.com/user-attachments/assets/0ddb1113-70c8-42d6-a16c-16b8042ec8d0" />

From left to right: Case with DIY projection lens (I skip this part – it worked but not really well), Sony E-Mount, C-Mount. In the middle the rest.

## LCD-Holder
A custom LCD-holder was designed to perfectly mount the LCD on the right place on the PCB.

<img width="373" height="295" alt="image" src="https://github.com/user-attachments/assets/4d8035ad-11a9-4181-9c1f-4dea6bf91e82" />
<img width="354" height="293" alt="image" src="https://github.com/user-attachments/assets/06ac888f-d65a-4fa3-b85f-ca86cb0fb6cc" />

## Case – C-Mount Variant
There are incredibly cheap chinase C-Mount lenses with a focal length of 25mm. They are sooo cheap that image quality is really really bad when using for fotography.

<img width="352" height="305" alt="image" src="https://github.com/user-attachments/assets/e3cfb40e-8b56-45df-a2db-5a8ab344a756" />

But surprisingly it works (almost) perfect for the Mini DIY Projector 😀

<img width="307" height="726" alt="image" src="https://github.com/user-attachments/assets/751fcf4f-08d7-44bd-bb08-c70b2dea0394" />

The pillow-effect (optical distortion where the edges are not straight but bent outwards) is almost neglectable – resulting a much better image than I expected (after reading reviews to this lense)

<img width="420" height="272" alt="image" src="https://github.com/user-attachments/assets/abf9e0a4-7616-435b-8975-f01bf3262283" />

## Case – Sony E-Mount Variant
Just for fun – and because I have a 3d printer which works reliably without much attention – I did a variant for Sony E-Mount.

<img width="397" height="340" alt="image" src="https://github.com/user-attachments/assets/4b9de40c-4640-4ac0-95d2-464b96bfaa77" />

Here the projector with Walimex Pro 10mm 2.8 lens.

<img width="264" height="333" alt="image" src="https://github.com/user-attachments/assets/052bbb07-4812-4238-afb6-ef5d62e18d58" />

I have to admit – it isn’t that bright in reality but it’s still very readable although it is so large.

<img width="407" height="270" alt="image" src="https://github.com/user-attachments/assets/eb756904-f63c-4ad7-96a3-56063effbc12" />

The image has more than 2m in width! And distortions almost non-existent 🙂

Downloads
GitHub-Repository contains:
– Schematic
– Layout
– Bill of material
– STM32 Cortex ARM Source
– 3D-STL-Files
– Images


Archive of original site: https://web.archive.org/web/20241128173803/https://microengineer.eu/2018/05/01/diy-night-clock-projector/
