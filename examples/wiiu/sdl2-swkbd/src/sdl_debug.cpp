/*
 * Print out all SDL2 events.
 */

#include <cstdio>
#include <string>

#include <SDL2/SDL.h>

#include "sdl_debug.hpp"


using std::printf;


namespace sdl::debug {

    void
    print(const SDL_AudioDeviceEvent& e)
    {
        const char* type_str = e.type == SDL_AUDIODEVICEADDED ? "added" : "removed";
        const char* name = SDL_GetAudioDeviceName(e.which, e.iscapture);
        printf("SDL_AudioDeviceEvent: type=%s, which=%u (%s), iscapture=%s\n",
               type_str,
               e.which,
               name,
               e.iscapture ? "yes" : "no");
    }


    void
    print(const SDL_ControllerAxisEvent& e)
    {
        const char* name = SDL_GameControllerNameForIndex(e.which);
        const char* axis_name = SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(e.axis));
        if (!axis_name)
            axis_name = "<NULL>";
        printf("SDL_ControllerAxisEvent: which=%d (%s), axis=%u (%s), value=%d\n",
               e.which,
               name,
               unsigned{e.axis},
               axis_name,
               int{e.value});
    }


    void
    print(const SDL_ControllerButtonEvent& e)
    {
        const char* name = SDL_GameControllerNameForIndex(e.which);
        const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
        auto button = static_cast<SDL_GameControllerButton>(e.button);
        const char* button_name = SDL_GameControllerGetStringForButton(button);
        if (!button_name)
            button_name = "<NULL>";
        printf("SDL_ControllerButtonEvent: which=%d (%s), button=%d (%s), state=%s\n",
               e.which,
               name,
               e.button,
               button_name,
               state);
    }


    void
    print(const SDL_ControllerDeviceEvent& e)
    {
        const char* name;
        const char* type;
        switch (e.type) {
            case SDL_CONTROLLERDEVICEADDED:
                type = "added";
                name = SDL_GameControllerNameForIndex(e.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                type = "removed";
                name = SDL_GameControllerName(SDL_GameControllerFromInstanceID(e.which));
                break;
            case SDL_CONTROLLERDEVICEREMAPPED:
                type = "remapped";
                name = SDL_GameControllerName(SDL_GameControllerFromInstanceID(e.which));
                break;
            default:
                type = "invalid";
                name = "<NULL>";
        }
        printf("SDL_ControllerDeviceEvent: type=%s, which=%d (%s)\n",
               type,
               e.which,
               name);
    }


    void
    print(const SDL_ControllerSensorEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* sensor;
        switch (e.sensor) {
            case SDL_SENSOR_UNKNOWN:
                sensor = "unknown";
                break;
            case SDL_SENSOR_ACCEL:
                sensor = "accel";
                break;
            case SDL_SENSOR_GYRO:
                sensor = "gyro";
                break;
            case SDL_SENSOR_ACCEL_L:
                sensor = "accel-l";
                break;
            case SDL_SENSOR_GYRO_L:
                sensor = "gyro-l";
                break;
            case SDL_SENSOR_ACCEL_R:
                sensor = "accel-r";
                break;
            case SDL_SENSOR_GYRO_R:
                sensor = "gyro-r";
                break;
            default:
                sensor = "invalid";
        }
        printf("SDL_ControllerSensorEvent: which=%d (%s), sensor=%s\n",
               e.which,
               name,
               sensor);
    }


    void
    print(const SDL_ControllerTouchpadEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* type;
        switch (e.type) {
            case SDL_CONTROLLERTOUCHPADDOWN:
                type = "down";
                break;
            case SDL_CONTROLLERTOUCHPADMOTION:
                type = "motion";
                break;
            case SDL_CONTROLLERTOUCHPADUP:
                type = "up";
                break;
            default:
                type = "invalid";
        }
        printf("SDL_ControllerTouchpadEvent: type=%s, which=%d (%s), touchpad=%d,"
               " x=%f, y=%f, pressure=%f\n",
               type,
               e.which,
               name,
               e.touchpad,
               e.x,
               e.y,
               e.pressure);
    }


    void
    print(const SDL_DisplayEvent& e)
    {
        const char* name = SDL_GetDisplayName(e.display);
        const char* event;
        switch (e.event) {
            case SDL_DISPLAYEVENT_ORIENTATION:
                event = "orientation";
                break;
            case SDL_DISPLAYEVENT_CONNECTED:
                event = "connected";
                break;
            case SDL_DISPLAYEVENT_DISCONNECTED:
                event = "disconnected";
                break;
            default:
                event = "invalid";
        }
        printf("SDL_DisplayEvent: display=%d (%s), event=%s\n",
               e.display,
               name,
               event);
    }


    void
    print(const SDL_DollarGestureEvent& e)
    {
        const char* type;
        switch (e.type) {
            case SDL_DOLLARGESTURE:
                type = "gesture";
                break;
            case SDL_DOLLARRECORD:
                type = "record";
                break;
            default:
                type = "invalid";
        }
        printf("SDL_DollarGestureEvent: type=%s, touchId=%" SDL_PRIs64 ", gestureId=%" SDL_PRIs64 ", numFingers=%u,"
               " error=%f, x=%f, y=%f\n",
               type,
               e.touchId,
               e.gestureId,
               e.numFingers,
               e.error,
               e.x,
               e.y);
    }


    void
    print(const SDL_DropEvent& e)
    {
        const char* type;
        switch (e.type) {
            case SDL_DROPBEGIN:
                type = "begin";
                break;
            case SDL_DROPFILE:
                type = "file";
                break;
            case SDL_DROPTEXT:
                type = "text";
                break;
            case SDL_DROPCOMPLETE:
                type = "complete";
                break;
            default:
                type = "invalid";
        }
        const char* file = e.file;
        if (!file)
            file = "<NULL>";
        printf("SDL_DropEvent: type=%s, file=\"%s\", windowID=%x\n",
               type,
               file,
               e.windowID);
    }


    namespace {

        void
        append(std::string& a, const std::string& b)
        {
            if (a.empty())
                a = b;
            else
                a += "," + b;
        }

        std::string
        keymod_to_string(Uint16 k)
        {
            std::string result;
            if (k & KMOD_LSHIFT)
                append(result, "LSHIFT");
            if (k & KMOD_RSHIFT)
                append(result, "RSHIFT");
            if (k & KMOD_LCTRL)
                append(result, "LCTRL");
            if (k & KMOD_RCTRL)
                append(result, "RCTRL");
            if (k & KMOD_LALT)
                append(result, "LALT");
            if (k & KMOD_RALT)
                append(result, "RALT");
            if (k & KMOD_LGUI)
                append(result, "LGUI");
            if (k & KMOD_RGUI)
                append(result, "RGUI");
            if (k & KMOD_NUM)
                append(result, "NUM");
            if (k & KMOD_CAPS)
                append(result, "CAPS");
            if (k & KMOD_MODE)
                append(result, "MODE");
            if (k & KMOD_SCROLL)
                append(result, "SCROLL");
            return result;
        }

    }


    void
    print(const SDL_KeyboardEvent& e)
    {
        const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
        const char* scancode_name = SDL_GetScancodeName(e.keysym.scancode);
        const char* key_name = SDL_GetKeyName(e.keysym.sym);
        std::string mod_name = keymod_to_string(e.keysym.mod);
        printf("SDL_KeyboardEvent: windowID=%x, state=%s, repeat=%s,\n"
               "\tkeysym={ scancode=%d (%s), sym=%x (%s), mod=[%s] }\n",
               e.windowID,
               state,
               e.repeat ? "yes" : "no",
               e.keysym.scancode,
               scancode_name,
               e.keysym.sym,
               key_name,
               mod_name.data());
    }


    void
    print(const SDL_JoyAxisEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        printf("SDL_JoyAxisEvent: which=%d (%s), axis=%u, value=%d\n",
               e.which,
               name,
               unsigned{e.axis},
               e.value);
    }


    void
    print(const SDL_JoyBallEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        printf("SDL_JoyBallEvent: which=%d (%s), ball=%u, xrel=%d, yrel=%d\n",
               e.which,
               name,
               unsigned{e.ball},
               e.xrel,
               e.yrel);
    }


    void
    print(const SDL_JoyBatteryEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* level;
        switch (e.level) {
            case SDL_JOYSTICK_POWER_EMPTY:
                level = "empty";
                break;
            case SDL_JOYSTICK_POWER_LOW:
                level = "low";
                break;
            case SDL_JOYSTICK_POWER_MEDIUM:
                level = "medium";
                break;
            case SDL_JOYSTICK_POWER_FULL:
                level = "full";
                break;
            case SDL_JOYSTICK_POWER_WIRED:
                level = "wired";
                break;
            default:
                level = "invalid";
        }
        printf("SDL_JoyBatteryEvent: which=%d (%s), level=%s\n",
               e.which,
               name,
               level);
    }


    void
    print(const SDL_JoyButtonEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
        printf("SDL_JoyButtonEvent: which=%d (%s), button=%u, state=%s\n",
               e.which,
               name,
               unsigned{e.button},
               state);
    }


    void
    print(const SDL_JoyDeviceEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* type = e.type == SDL_JOYDEVICEADDED ? "added" : "removed";
        printf("SDL_JoyDeviceEvent: type=%s, which=%d (%s)\n",
               type,
               e.which,
               name);
    }


    void
    print(const SDL_JoyHatEvent& e)
    {
        const char* name = SDL_JoystickNameForIndex(e.which);
        const char* value;
        switch (e.value) {
            case SDL_HAT_CENTERED:
                value = "∘";//
                break;
            case SDL_HAT_UP:
                value = "↑";
                break;
            case SDL_HAT_RIGHT:
                value = "→";
                break;
            case SDL_HAT_DOWN:
                value = "↓";
                break;
            case SDL_HAT_LEFT:
                value = "←";
                break;
            case SDL_HAT_RIGHTUP:
                value = "↗";
                break;
            case SDL_HAT_RIGHTDOWN:
                value = "↘";
                break;
            case SDL_HAT_LEFTUP:
                value = "↖";
                break;
            case SDL_HAT_LEFTDOWN:
                value = "↙";
                break;
            default:
                value = "invalid";
        }
        printf("SDL_JoyHatEvent: which=%d (%s), hat=%u, value=%s\n",
               e.which,
               name,
               unsigned{e.hat},
               value);
    }


    void
    print(const SDL_MouseButtonEvent& e)
    {
        const char* state = e.state == SDL_PRESSED ? "pressed" : "released";
        printf("SDL_MouseButtonEvent: windowID=%x, which=%u, button=%u,"
               " state=%s, clicks=%u, x=%d, y=%d\n",
               e.windowID,
               e.which,
               unsigned{e.button},
               state,
               unsigned{e.clicks},
               e.x,
               e.y);
    }

    void
    print(const SDL_MouseMotionEvent& e)
    {
        printf("SDL_MouseMotionEvent: windowID=%x, which=%u, state=%x, x=%d, y=%d,"
               " xrel=%d, yrel=%d\n",
               e.windowID,
               e.which,
               e.state,
               e.x,
               e.y,
               e.xrel,
               e.yrel);
    }


    void
    print(const SDL_MouseWheelEvent& e)
    {
        const char* direction = e.direction == SDL_MOUSEWHEEL_NORMAL ? "normal" : "flipped";
        printf("SDL_MouseWheelEvent: windowID=%x, which=%u, direction=%s,"
               " x=%d, y=%d, preciseX=%f, preciseY=%f, mouseX=%d, mouseY=%d\n",
               e.windowID,
               e.which,
               direction,
               e.x,
               e.y,
               e.preciseX,
               e.preciseY,
               e.mouseX,
               e.mouseY);
    }


    void
    print(const SDL_MultiGestureEvent& e)
    {
        printf("SDL_MultiGestureEvent: touchId=%" SDL_PRIs64 ", dTheta=%f, dDist=%f, "
               "x=%f, y=%f, numFingers=%u\n",
               e.touchId,
               e.dTheta,
               e.dDist,
               e.x,
               e.y,
               e.numFingers);
    }


    void
    print(const SDL_QuitEvent&)
    {
        printf("SDL_QuitEvent\n");
    }


    void
    print(const SDL_SensorEvent& e)
    {
        const char* name = "<NULL>";
        auto sensor = SDL_SensorFromInstanceID(e.which);
        if (sensor)
            name = SDL_SensorGetName(sensor);
        printf("SDL_SensorEvent: which=%d (%s)\n",
               e.which,
               name);
    }


    namespace {

        std::string
        repr_editing_text(const char* txt,
                          int start,
                          int length)
        {
            std::string result = txt;
            result.insert(start + length, 1, ']');
            result.insert(start, 1, '[');
            return result;
        }

    }


    void
    print(const SDL_TextEditingEvent& e)
    {
        std::string text = repr_editing_text(e.text, e.start, e.length);
        printf("SDL_TextEditingEvent: windowID=%x, text=\"%s\"\n",
               e.windowID,
               text.data());
    }


    void
    print(const SDL_TextEditingExtEvent& e)
    {
        std::string text = repr_editing_text(e.text, e.start, e.length);
        printf("SDL_TextEditingExtEvent: windowID=%x, text=\"%s\"\n",
               e.windowID,
               text.data());
    }


    void
    print(const SDL_TextInputEvent& e)
    {
        printf("SDL_TextInputEvent: windowID=%x, text=\"%s\"\n",
               e.windowID,
               e.text);
    }


    void
    print(const SDL_TouchFingerEvent& e)
    {
        const char* type;
        switch (e.type) {
            case SDL_FINGERMOTION:
                type = "motion";
                break;
            case SDL_FINGERDOWN:
                type = "down";
                break;
            case SDL_FINGERUP:
                type = "up";
                break;
            default:
                type = "invalid";
        }
        printf("SDL_TouchFingerEvent: type=%s, touchId=%" SDL_PRIs64 ", fingerId=%" SDL_PRIs64 ","
               " x=%f, y=%f, dx=%f, dy=%f, pressure=%f, windowID=%u\n",
               type,
               e.touchId,
               e.fingerId,
               e.x,
               e.y,
               e.dx,
               e.dy,
               e.pressure,
               e.windowID);
    }


    void
    print(const SDL_WindowEvent& e)
    {
        const char* event;
        switch (e.event) {
            case SDL_WINDOWEVENT_SHOWN:
                event = "shown";
                break;
            case SDL_WINDOWEVENT_HIDDEN:
                event = "hidden";
                break;
            case SDL_WINDOWEVENT_EXPOSED:
                event = "exposed";
                break;
            case SDL_WINDOWEVENT_MOVED:
                event = "moved";
                break;
            case SDL_WINDOWEVENT_RESIZED:
                event = "resized";
                break;
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                event = "size-changed";
                break;
            case SDL_WINDOWEVENT_MINIMIZED:
                event = "minimized";
                break;
            case SDL_WINDOWEVENT_MAXIMIZED:
                event = "maximized";
                break;
            case SDL_WINDOWEVENT_RESTORED:
                event = "restored";
                break;
            case SDL_WINDOWEVENT_ENTER:
                event = "enter";
                break;
            case SDL_WINDOWEVENT_LEAVE:
                event = "leavel";
                break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
                event = "focus-gained";
                break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
                event = "focus-lost";
                break;
            case SDL_WINDOWEVENT_CLOSE:
                event = "close";
                break;
            case SDL_WINDOWEVENT_TAKE_FOCUS:
                event = "take-focus";
                break;
            case SDL_WINDOWEVENT_HIT_TEST:
                event = "hit-test";
                break;
            case SDL_WINDOWEVENT_ICCPROF_CHANGED:
                event = "iccprof-changed";
                break;
            case SDL_WINDOWEVENT_DISPLAY_CHANGED:
                event = "display-changed";
                break;
            default:
                event = "invalid";
        }
        printf("SDL_WindowEvent: windowID=%x, event=%s\n",
               e.windowID,
               event);
    }


    void
    print(const char* type)
    {
        printf("SDL_Event: type=%s\n", type);
    }


    void
    print(const SDL_Event& e)
    {
        switch (e.type) {

            case SDL_QUIT:
                print(e.quit);
                break;

            case SDL_DISPLAYEVENT:
                print(e.display);
                break;

            case SDL_WINDOWEVENT:
                print(e.window);
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
                print(e.key);
                break;

            case SDL_TEXTEDITING:
                print(e.edit);
                break;

            case SDL_TEXTINPUT:
                print(e.text);
                break;

            case SDL_TEXTEDITING_EXT:
                print(e.editExt);
                break;

            case SDL_MOUSEMOTION:
                print(e.motion);
                break;

            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                print(e.button);
                break;

            case SDL_MOUSEWHEEL:
                print(e.wheel);
                break;

            case SDL_JOYAXISMOTION:
                print(e.jaxis);
                break;

            case SDL_JOYBALLMOTION:
                print(e.jball);
                break;

            case SDL_JOYHATMOTION:
                print(e.jhat);
                break;

            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                print(e.jbutton);
                break;

            case SDL_JOYDEVICEADDED:
            case SDL_JOYDEVICEREMOVED:
                print(e.jdevice);
                break;

            case SDL_JOYBATTERYUPDATED:
                print(e.jbattery);
                break;

            case SDL_CONTROLLERAXISMOTION:
                print(e.caxis);
                break;

            case SDL_CONTROLLERBUTTONDOWN:
            case SDL_CONTROLLERBUTTONUP:
                print(e.cbutton);
                break;

            case SDL_CONTROLLERDEVICEADDED:
            case SDL_CONTROLLERDEVICEREMOVED:
            case SDL_CONTROLLERDEVICEREMAPPED:
                print(e.cdevice);
                break;

            case SDL_CONTROLLERTOUCHPADDOWN:
            case SDL_CONTROLLERTOUCHPADMOTION:
            case SDL_CONTROLLERTOUCHPADUP:
                print(e.ctouchpad);
                break;

            case SDL_CONTROLLERSENSORUPDATE:
                print(e.csensor);
                break;

            case SDL_FINGERDOWN:
            case SDL_FINGERUP:
            case SDL_FINGERMOTION:
                print(e.tfinger);
                break;

            case SDL_DOLLARGESTURE:
            case SDL_DOLLARRECORD:
                print(e.dgesture);
                break;

            case SDL_MULTIGESTURE:
                print(e.mgesture);
                break;


            case SDL_DROPFILE:
            case SDL_DROPTEXT:
            case SDL_DROPBEGIN:
            case SDL_DROPCOMPLETE:
                print(e.drop);
                break;

            case SDL_AUDIODEVICEADDED:
            case SDL_AUDIODEVICEREMOVED:
                print(e.adevice);
                break;

            case SDL_SENSORUPDATE:
                print(e.sensor);
                break;


            // Here are the events that have no data associated with them.

#define EMPTY_CASE(x)                           \
    case x:                                     \
        print(#x);                              \
        break

            EMPTY_CASE(SDL_APP_DIDENTERBACKGROUND);
            EMPTY_CASE(SDL_APP_DIDENTERFOREGROUND);
            EMPTY_CASE(SDL_APP_LOWMEMORY);
            EMPTY_CASE(SDL_APP_TERMINATING);
            EMPTY_CASE(SDL_APP_WILLENTERBACKGROUND);
            EMPTY_CASE(SDL_APP_WILLENTERFOREGROUND);
            EMPTY_CASE(SDL_CLIPBOARDUPDATE);
            EMPTY_CASE(SDL_KEYMAPCHANGED);
            EMPTY_CASE(SDL_LOCALECHANGED);
            EMPTY_CASE(SDL_RENDER_DEVICE_RESET);
            EMPTY_CASE(SDL_RENDER_TARGETS_RESET);
            EMPTY_CASE(SDL_SYSWMEVENT);

#undef EMPTY_CASE

            default:
                ;
        }
    }



} // namespace sdl::debug
