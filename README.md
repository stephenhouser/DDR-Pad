---
category: woodworking
---

## Todo

- √ Clear acrylic ~ 12x12x 0.354 @ 10 = 180 + $50 shipping
 - https://www.tapplastics.com/product/plastics/cut_to_size_plastic/polycarbonate_sheets/516
 - 2024/01/01 - orderd 10 11x11x0.354" (3/8")
 - $229.94
- √ M6 Screws, M6 Nylon nuts -- Granger
 - 2024/01/03 Picked up nuts, screws on backorder to 2024/01/08
 - $60
	
- Decals ~ $70
 - https://www.bannerbuzz.com/clear-window-decals/p

- Arduino/teensy -- will it be fast enough?
 - perhaps microcontrollers at each pad sending data and lights?

- Metal corners?
 - 16 gague, need sheet brake (Rex?)

- 1.25" SteelTex no-thread pipe -- will it be sturdy enough
 - 1.25 is only in silver color
 - 2 bases, 2 90-deg, 3 poles

## The Build we are working from

- The hardware: [DIY Arcade DDR Metal Dance Pad With LED Under $500](https://www.instructables.com/DIY-Arcade-DDR-Metal-Dance-Pad-With-LED-Under-500/)
- The electronics: [6 Panel DDR USB Controller With LED](https://www.instructables.com/6-Panel-DDR-USB-Controller-With-LED/)

## Other Builds

- [Mystic Steps build](https://www.mysticsteps.com/diy-pad/), uses a AtMega32U (32u chipset for USB I imagine)
- [andrewhlu's build](https://andrewhlu.com/ddr/), uses [STM32L476G board](https://www.digikey.com/en/products/detail/stmicroelectronics/STM32L476G-DISCO/5344355)
- [Sensor Channels](https://www.thingiverse.com/thing:4547247) on Thingiverse, replacements for original DDR

## Making Pads

- https://www.hackmycab.com/?portfolio=arrow-panels-part-1-making-new-polycarb-panels
- https://www.hackmycab.com/?portfolio=arrow-panels-part-2-arrow-decals

## Lights

- [8x6 Lights](https://ddrpad.com/collections/arcade-parts/products/arcade-pad-led-light-mod) for original DDR
- [8x8 addressable LED modules](https://www.amazon.com/WS2812B-Flexible-Individually-Addressable-64Pixels/dp/B09KBZT6LD/ref=sr_1_4?crid=2BYV3RSFGN8D9&keywords=addressable%2Brgb%2Bled%2Bpanel&qid=1704314467&sprefix=addressable%2Brgb%2Bled%2Bpanel%2Caps%2C105&sr=8-4&th=1) from Amazon
 - ws2812 should be able to do about 64fps with 512 in a chain.
 - will need power injection at each row?
 - https://electronics.stackexchange.com/questions/338248/ws2812b-response-time
