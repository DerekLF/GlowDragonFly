# GLOW 2024 Dragonfly

GLOW Eindhoven is an annual light art festival held in Eindhoven, Netherlands, showcasing innovative light installations and artworks from local and international artists. The event transforms the city into a vibrant, illuminated space, celebrating creativity and technology. Visitors can explore a diverse range of light projects that engage and inspire, making it a unique cultural experience. For more details, you can visit the official GLOW website.

**Programmer**: le moi
The dragonflies use an RGBW (SK2812) addressable LED strip controlled by an ESP32. The body is divided into 3 parts: 4 wings, a tail, and a head.
```cpp
//LED Count for Each Body Part:  
Num_Leds_Wings = 34  
Num_Leds_Head = 6  
Num_Leds_Tail = 31
```
## Modes

The dragonfly has several lighting modes:

### `mode_Static` Function

This function sets a static color for the dragonfly's LEDs, allowing for a single color display at a specified brightness.

- **Parameters**:
  - `colour`: The color to be displayed, typically represented as an integer value.
  - `brightness`: The brightness level of the color display.

- **Working**:
  - The function calls `lightLED` with the specified color and brightness, turning on the LEDs to show a solid color. This mode can be used for a variety of visual effects or to set the dragonfly's LEDs to a single color for simplicity.
  
This static mode provides a straightforward way to illuminate the dragonfly with a consistent color, enhancing its visibility and aesthetics.

### `mode_Static_P` Function

This function implements a static color mode for the dragonfly's wings, displaying two distinct colors across the LED strips.

- **Parameters**:
  - `arDATA`: An array containing color and brightness data for two different static colors.

- **Working**:
  - The function first calls the `lightLED` function to determine the RGB values for the first color based on the input from `arDATA[0]`. It then fills the first half of the wings (both left and right) with this color.
  - Next, it calls `lightLED` again to get the RGB values for the second color from `arDATA[1]`. The second half of the wings is then filled with this color.
  - The function covers all LEDs in the wings by dividing them into two halves, ensuring a smooth transition between the two colors.

This static color mode enhances the visual appeal of the dragonfly, creating a striking two-tone effect on its wings.

### `mode_Travel_1` Function

This function implements the "Travel 1" lighting mode, where light travels from the tip of the tail to the head of the dragonfly. It creates a dynamic lighting effect that sequentially activates the LEDs from the tail to the wings and finally to the head.

- **Parameters**:
  - `intervalT1`: The time interval that controls how fast the light travels from the tail to the head.
  - `arDATA`: An array containing color and brightness data for the LEDs.

- **Working**:
  - The function uses `millis()` to track the time elapsed and updates the LED colors based on the current state of the animation.
  - The `state` variable determines which part of the dragonfly is currently being lit:
    - **State 0**: Sets the background color for the LEDs.
    - **State 1**: Lights up the tail from the end to the tip, creating a travel effect.
    - **State 2**: Activates the back wings, lighting them from beginning to end.
    - **State 3**: Activates the front wings, again lighting them from beginning to end.
    - **State 4**: Fills the head (eyes) with color, gradually increasing the brightness until a threshold is reached.
  - Once the light has traveled through all the components (tail, wings, and head), the function resets the position and increments the state to repeat the cycle.
  - The function returns a value (`returnValT1`) indicating whether the full travel cycle is complete.

This mode creates a visually engaging effect, making the dragonfly appear animated and lively as the light moves through its body.

### `mode_Travel_2` Function

This function controls the "Travel 2" lighting mode, where light travels from the dragonfly's body to the ends of the wings and tail. It uses two colors: a background color and a moving color that progresses through the LED strips.

- **Parameters**:
  - `interval`: The time interval that controls how fast the light moves.
  - `mode`: Defines the behavior of the light effect (e.g., whether to reset positions).
  - `arDATA`: An array holding color and brightness data.

- **Working**:
  - The function calculates the current time using `millis()` and updates different parts of the dragonfly (head, wings, and tail) based on this interval.
  - It fills the head, tail, and wings with the background color, then overlays a traveling color that moves forward, creating the effect of movement.
  - The position of the moving color is tracked for both the wings and tail using `positionT2W` and `positionT2T`. Once the light has traveled across all LEDs in the wings and tail, the function resets the positions to loop the effect.
  - The function returns a value (`returnValT2`) that indicates when the full travel cycle is complete.

The light effect is continuously updated at a rate determined by the `interval` and the number of LEDs in the wings and tail, ensuring a smooth animation.

### `mode_Heartbeat` Function

This function simulates a heartbeat effect on the dragonfly's wings. The LEDs pulse between a low brightness and a peak brightness, mimicking the rhythm of a heartbeat.

- **Parameters**:
  - `BPM`: The number of beats per minute, used to control the timing of the heartbeat effect. This value is converted to milliseconds.

- **Working**:
  - The function uses `millis()` to measure the time elapsed between beats and toggles between two states: low brightness (resting state) and peak brightness (heartbeat).
  - The LED brightness alternates between a random low value and a peak value based on the heartbeat rate (`BPM`).
  - A switch case is used to determine which brightness level to apply (`Low` or `Peak`), and the `lightLED` function is called accordingly to light up the LEDs.

The heartbeat is updated each time the elapsed time matches the interval for the specified BPM, ensuring the LED pattern is synchronized to the heartbeat rate.

  
- Spiral: Lights spiral through the wings, tail, and head. --to do
### `mode_Wing` Function

This function controls the lighting effect for the dragonfly's wings, simulating the gradual flapping motion. The wings are lit with a background color and an additional "traveling" color that moves across the LEDs.

- **Parameters**:
  - `speed`: Controls the timing between each LED update (flapping speed).
  - `arDATA`: An array containing data for colors and brightness.

- **Working**:
  - First, the background color is applied to all the LEDs on the wings using `lightLED`.
  - A second color (traveling color) then moves across the wings at the specified speed, creating the visual effect of movement.
  - The timing between updates is managed using `millis()`, ensuring the function runs without blocking the main loop.

This effect is spread across the four wings (`wingLF`, `wingRF`, `wingLB`, `wingRB`), and the lighting is continuously updated based on the `beginMC` position, which keeps track of where the light effect starts on the wing.


## Colour Conversion Functions

Two functions are used for color conversion:

```cpp 
fetchColourCombo(int colourPair, int* returnArr)
```
This function returns two matching colors and their respective brightness settings through the returnArr array.

```cpp 
lightLED(int colorCode, int mode, int brightness, int* returnArRGB)
```
This function takes the chosen color code, mode, and brightness, and outputs the corresponding RGBW values.
