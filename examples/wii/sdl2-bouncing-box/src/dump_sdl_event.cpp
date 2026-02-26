#include <stdio.h>
#include <string>

#include <SDL_stdinc.h>
#include <SDL_touch.h>

#include "dump_sdl_event.hpp"


using namespace std::literals;


static
std::string
event_type_to_string(Uint32 type)
{
    switch (type) {
        case SDL_FIRSTEVENT:
            return "SDL_FIRSTEVENT";
        case SDL_QUIT:
            return "SDL_QUIT";
        case SDL_APP_TERMINATING:
            return "SDL_APP_TERMINATING";
        case SDL_APP_LOWMEMORY:
            return "SDL_APP_LOWMEMORY";
        case SDL_APP_WILLENTERBACKGROUND:
            return "SDL_APP_WILLENTERBACKGROUND";
        case SDL_APP_DIDENTERBACKGROUND:
            return "SDL_APP_DIDENTERBACKGROUND";
        case SDL_APP_WILLENTERFOREGROUND:
            return "SDL_APP_WILLENTERFOREGROUND";
        case SDL_APP_DIDENTERFOREGROUND:
            return "SDL_APP_DIDENTERFOREGROUND";
        case SDL_LOCALECHANGED:
            return "SDL_LOCALECHANGED";
        case SDL_DISPLAYEVENT:
            return "SDL_DISPLAYEVENT";
        case SDL_WINDOWEVENT:
            return "SDL_WINDOWEVENT";
        case SDL_SYSWMEVENT:
            return "SDL_SYSWMEVENT";
        case SDL_KEYDOWN:
            return "SDL_KEYDOWN";
        case SDL_KEYUP:
            return "SDL_KEYUP";
        case SDL_TEXTEDITING:
            return "SDL_TEXTEDITING";
        case SDL_TEXTINPUT:
            return "SDL_TEXTINPUT";
        case SDL_KEYMAPCHANGED:
            return "SDL_KEYMAPCHANGED";
        case SDL_TEXTEDITING_EXT:
            return "SDL_TEXTEDITING_EXT";
        case SDL_MOUSEMOTION:
            return "SDL_MOUSEMOTION";
        case SDL_MOUSEBUTTONDOWN:
            return "SDL_MOUSEBUTTONDOWN";
        case SDL_MOUSEBUTTONUP:
            return "SDL_MOUSEBUTTONUP";
        case SDL_MOUSEWHEEL:
            return "SDL_MOUSEWHEEL";
        case SDL_JOYAXISMOTION:
            return "SDL_JOYAXISMOTION";
        case SDL_JOYBALLMOTION:
            return "SDL_JOYBALLMOTION";
        case SDL_JOYHATMOTION:
            return "SDL_JOYHATMOTION";
        case SDL_JOYBUTTONDOWN:
            return "SDL_JOYBUTTONDOWN";
        case SDL_JOYBUTTONUP:
            return "SDL_JOYBUTTONUP";
        case SDL_JOYDEVICEADDED:
            return "SDL_JOYDEVICEADDED";
        case SDL_JOYDEVICEREMOVED:
            return "SDL_JOYDEVICEREMOVED";
        case SDL_JOYBATTERYUPDATED:
            return "SDL_JOYBATTERYUPDATED";
        case SDL_CONTROLLERAXISMOTION:
            return "SDL_CONTROLLERAXISMOTION";
        case SDL_CONTROLLERBUTTONDOWN:
            return "SDL_CONTROLLERBUTTONDOWN";
        case SDL_CONTROLLERBUTTONUP:
            return "SDL_CONTROLLERBUTTONUP";
        case SDL_CONTROLLERDEVICEADDED:
            return "SDL_CONTROLLERDEVICEADDED";
        case SDL_CONTROLLERDEVICEREMOVED:
            return "SDL_CONTROLLERDEVICEREMOVED";
        case SDL_CONTROLLERDEVICEREMAPPED:
            return "SDL_CONTROLLERDEVICEREMAPPED";
        case SDL_CONTROLLERTOUCHPADDOWN:
            return "SDL_CONTROLLERTOUCHPADDOWN";
        case SDL_CONTROLLERTOUCHPADMOTION:
            return "SDL_CONTROLLERTOUCHPADMOTION";
        case SDL_CONTROLLERTOUCHPADUP:
            return "SDL_CONTROLLERTOUCHPADUP";
        case SDL_CONTROLLERSENSORUPDATE:
            return "SDL_CONTROLLERSENSORUPDATE";
        case SDL_FINGERDOWN:
            return "SDL_FINGERDOWN";
        case SDL_FINGERUP:
            return "SDL_FINGERUP";
        case SDL_FINGERMOTION:
            return "SDL_FINGERMOTION";
        case SDL_DOLLARGESTURE:
            return "SDL_DOLLARGESTURE";
        case SDL_DOLLARRECORD:
            return "SDL_DOLLARRECORD";
        case SDL_MULTIGESTURE:
            return "SDL_MULTIGESTURE";
        case SDL_CLIPBOARDUPDATE:
            return "SDL_CLIPBOARDUPDATE";
        case SDL_DROPFILE:
            return "SDL_DROPFILE";
        case SDL_DROPTEXT:
            return "SDL_DROPTEXT";
        case SDL_DROPBEGIN:
            return "SDL_DROPBEGIN";
        case SDL_DROPCOMPLETE:
            return "SDL_DROPCOMPLETE";
        case SDL_AUDIODEVICEADDED:
            return "SDL_AUDIODEVICEADDED";
        case SDL_AUDIODEVICEREMOVED:
            return "SDL_AUDIODEVICEREMOVED";
        case SDL_SENSORUPDATE:
            return "SDL_SENSORUPDATE";
        case SDL_RENDER_TARGETS_RESET:
            return "SDL_RENDER_TARGETS_RESET";
        case SDL_RENDER_DEVICE_RESET:
            return "SDL_RENDER_DEVICE_RESET";
        case SDL_POLLSENTINEL:
            return "SDL_POLLSENTINEL";
        default:
            if (type >= SDL_USEREVENT && type <= SDL_LASTEVENT)
                return "SDL_USEREVENT(" + std::to_string(type - SDL_USEREVENT) + ")";
    }
    return "invalid(" + std::to_string(type) + ")";
}


static
void
dump_sdl_event(const SDL_AudioDeviceEvent& e)
{
    printf("%s: which=%u, iscapture=%u\n",
           event_type_to_string(e.type).data(),
           e.which,
           unsigned{e.iscapture});
}


static
const char*
controller_name_from_instance_id(SDL_JoystickID id)
{
    auto c = SDL_GameControllerFromInstanceID(id);
    if (!c)
        return "<NULL>";
    auto result = SDL_GameControllerName(c);
    if (!result)
        return "<NULL>";
    return result;
}


static
std::string
controller_axis_to_string(int axis)
{
    switch (static_cast<SDL_GameControllerAxis>(axis)) {
        case SDL_CONTROLLER_AXIS_INVALID:
            return "SDL_CONTROLLER_AXIS_INVALID";
        case SDL_CONTROLLER_AXIS_LEFTX:
            return "SDL_CONTROLLER_AXIS_LEFTX";
        case SDL_CONTROLLER_AXIS_LEFTY:
            return "SDL_CONTROLLER_AXIS_LEFTY";
        case SDL_CONTROLLER_AXIS_RIGHTX:
            return "SDL_CONTROLLER_AXIS_RIGHTX";
        case SDL_CONTROLLER_AXIS_RIGHTY:
            return "SDL_CONTROLLER_AXIS_RIGHTY";
        case SDL_CONTROLLER_AXIS_TRIGGERLEFT:
            return "SDL_CONTROLLER_AXIS_TRIGGERLEFT";
        case SDL_CONTROLLER_AXIS_TRIGGERRIGHT:
            return "SDL_CONTROLLER_AXIS_TRIGGERRIGHT";
        case SDL_CONTROLLER_AXIS_MAX:
            return "SDL_CONTROLLER_AXIS_MAX";
    }
    return "invalid(" + std::to_string(axis) + ")";
}


static
void
dump_sdl_event(const SDL_ControllerAxisEvent& e)
{
    printf("%s: which=%d, name=\"%s\", axis=%s, value=%d\n",
           event_type_to_string(e.type).data(),
           e.which,
           controller_name_from_instance_id(e.which),
           controller_axis_to_string(e.axis).data(),
           int{e.value});
}


static
std::string
button_state_to_string(Uint32 state)
{
    switch (state) {
        case SDL_PRESSED:
            return "SDL_PRESSED";
        case SDL_RELEASED:
            return "SDL_RELEASED";
    }
    return "invalid(" + std::to_string(state) + ")";
}


static
void
dump_sdl_event(const SDL_ControllerButtonEvent& e)
{
    const char* button_name =
        SDL_GameControllerGetStringForButton(static_cast<SDL_GameControllerButton>(e.button));
    if (!button_name)
        button_name = "<NULL>";

    printf("%s: which=%d, name=\"%s\" button=%s, state=%s\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           controller_name_from_instance_id(e.which),
           button_name,
           button_state_to_string(e.state).data());
}


static
void
dump_sdl_event(const SDL_ControllerDeviceEvent& e)
{
    const char* name = nullptr;
    if (e.type == SDL_CONTROLLERDEVICEADDED)
        name = SDL_GameControllerNameForIndex(e.which);
    else
        name = controller_name_from_instance_id(e.which);
    if (!name)
        name = "<NULL>";
    printf("%s: which=%d, name=\"%s\"\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           name);
}


static
std::string
sensor_type_to_string(Sint32 type)
{
    switch (type) {
        case SDL_SENSOR_UNKNOWN:
            return "SDL_SENSOR_UNKNOWN";
        case SDL_SENSOR_ACCEL:
            return "SDL_SENSOR_ACCEL";
        case SDL_SENSOR_GYRO:
            return "SDL_SENSOR_GYRO";
        case SDL_SENSOR_ACCEL_L:
            return "SDL_SENSOR_ACCEL_L";
        case SDL_SENSOR_GYRO_L:
            return "SDL_SENSOR_GYRO_L";
        case SDL_SENSOR_ACCEL_R:
            return "SDL_SENSOR_ACCEL_R";
        case SDL_SENSOR_GYRO_R:
            return "SDL_SENSOR_GYRO_R";
    }
    return "invalid(" + std::to_string(type) + ")";
}


static
void
dump_sdl_event(const SDL_ControllerSensorEvent& e)
{
    printf("%s: which=%d, name=\"%s\", sensor=\"%s\"\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           controller_name_from_instance_id(e.which),
           sensor_type_to_string(e.sensor).data());
}


static
void
dump_sdl_event(const SDL_ControllerTouchpadEvent& e)
{
    printf("%s: which=%d, name=\"%s\", touchpad=%d, x=%f, y=%f, pressure=%f\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           controller_name_from_instance_id(e.which),
           e.touchpad,
           e.x,
           e.y,
           e.pressure);
}


static
std::string
display_event_to_string(Uint32 type)
{
    switch (type) {
        case SDL_DISPLAYEVENT_NONE:
            return "SDL_DISPLAYEVENT_NONE";
        case SDL_DISPLAYEVENT_ORIENTATION:
            return "SDL_DISPLAYEVENT_ORIENTATION";
        case SDL_DISPLAYEVENT_CONNECTED:
            return "SDL_DISPLAYEVENT_CONNECTED";
        case SDL_DISPLAYEVENT_DISCONNECTED:
            return "SDL_DISPLAYEVENT_DISCONNECTED";
        case SDL_DISPLAYEVENT_MOVED:
            return "SDL_DISPLAYEVENT_MOVED";
    }
    return "invalid(" + std::to_string(type) + ")";
}


static
void
dump_sdl_event(const SDL_DisplayEvent& e)
{
    printf("%s: display=%u, event=%s\n",
           event_type_to_string(e.type).data(),
           e.display,
           display_event_to_string(e.event).data());
}


static
const char*
joystick_name_from_instance_id(SDL_JoystickID id)
{
    auto j = SDL_JoystickFromInstanceID(id);
    if (!j)
        return "<NULL>";
    auto result = SDL_JoystickName(j);
    if (!result)
        return "<NULL>";
    return result;
}


static
void
dump_sdl_event(const SDL_JoyAxisEvent& e)
{
    printf("%s: which=%d, name=\"%s\", axis=%u, value=%d\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           joystick_name_from_instance_id(e.which),
           unsigned{e.axis},
           e.value);
}


static
void
dump_sdl_event(const SDL_JoyBallEvent& e)
{
    printf("%s: which=%d, name=\"%s\", ball=%u, xrel=%d, yrel=%d\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           joystick_name_from_instance_id(e.which),
           unsigned{e.ball},
           int{e.xrel},
           int{e.yrel});
}


static
std::string
joystick_power_to_string(SDL_JoystickPowerLevel level)
{
    switch (level) {
        case SDL_JOYSTICK_POWER_UNKNOWN:
            return "SDL_JOYSTICK_POWER_UNKNOWN";
        case SDL_JOYSTICK_POWER_EMPTY:
            return "SDL_JOYSTICK_POWER_EMPTY";
        case SDL_JOYSTICK_POWER_LOW:
            return "SDL_JOYSTICK_POWER_LOW";
        case SDL_JOYSTICK_POWER_MEDIUM:
            return "SDL_JOYSTICK_POWER_MEDIUM";
        case SDL_JOYSTICK_POWER_FULL:
            return "SDL_JOYSTICK_POWER_FULL";
        case SDL_JOYSTICK_POWER_WIRED:
            return "SDL_JOYSTICK_POWER_WIRED";
        case SDL_JOYSTICK_POWER_MAX:
            return "SDL_JOYSTICK_POWER_MAX";
    }
    return "invalid(" + std::to_string(static_cast<unsigned>(level)) + ")";
}


static
void
dump_sdl_event(const SDL_JoyBatteryEvent& e)
{
    printf("%s: which=%d, name=\"%s\", level=%s\n",
           event_type_to_string(e.type).data(),
           e.which,
           joystick_name_from_instance_id(e.which),
           joystick_power_to_string(e.level).data());
}


static
void
dump_sdl_event(const SDL_JoyButtonEvent& e)
{
    printf("%s: which=%d, name=\"%s\", button=%u, state=%s\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           joystick_name_from_instance_id(e.which),
           unsigned{e.button},
           button_state_to_string(e.state).data());
}


static
void
dump_sdl_event(const SDL_JoyDeviceEvent& e)
{
    const char* name = nullptr;
    if (e.type == SDL_JOYDEVICEADDED)
        name = SDL_JoystickNameForIndex(e.which);
    else
        name = joystick_name_from_instance_id(e.which);
    if (!name)
        name = "<NULL>";
    printf("%s: which=%d, name=\"%s\"\n",
           event_type_to_string(e.type).data(),
           e.which,
           name);
}


static
std::string
hat_dir_to_string(Uint32 dir)
{
    switch (dir) {
        case SDL_HAT_CENTERED:
            return "SDL_HAT_CENTERED";
        case SDL_HAT_UP:
            return "SDL_HAT_UP";
        case SDL_HAT_RIGHT:
            return "SDL_HAT_RIGHT";
        case SDL_HAT_DOWN:
            return "SDL_HAT_DOWN";
        case SDL_HAT_LEFT:
            return "SDL_HAT_LEFT";
        case SDL_HAT_RIGHTUP:
            return "SDL_HAT_RIGHTUP";
        case SDL_HAT_RIGHTDOWN:
            return "SDL_HAT_RIGHTDOWN";
        case SDL_HAT_LEFTUP:
            return "SDL_HAT_LEFTUP";
        case SDL_HAT_LEFTDOWN:
            return "SDL_HAT_LEFTDOWN";
    }
    return "invalid(" + std::to_string(dir) + ")";
}


static
void
dump_sdl_event(const SDL_JoyHatEvent& e)
{
    printf("%s: which=%d, name=\"%s\", hat=%u, value=%s\n",
           event_type_to_string(e.type).data(),
           int{e.which},
           joystick_name_from_instance_id(e.which),
           unsigned{e.hat},
           hat_dir_to_string(e.value).data());
}


static
std::string
get_mouse_name(Uint32 id)
{
    if (id == SDL_TOUCH_MOUSEID)
        return "touch";
    return std::to_string(id);
}


static
void
dump_sdl_event(const SDL_MouseButtonEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", which=%s, button=%u,"
           " state=%s, clicks=%u. x=%d, y=%d\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           get_mouse_name(e.which).data(),
           unsigned{e.button},
           button_state_to_string(e.state).data(),
           unsigned{e.clicks},
           e.x,
           e.y);
}


static
void
dump_sdl_event(const SDL_MouseMotionEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", which=%s, state=%x, x=%d, y=%d,"
           " xrel=%d, yrel=%d\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           get_mouse_name(e.which).data(),
           e.state,
           e.x,
           e.y,
           e.xrel,
           e.yrel);
}


static
std::string
wheel_dir_to_string(Uint32 dir)
{
    switch (dir) {
        case SDL_MOUSEWHEEL_NORMAL:
            return "SDL_MOUSEWHEEL_NORMAL";
        case SDL_MOUSEWHEEL_FLIPPED:
            return "SDL_MOUSEWHEEL_FLIPPED";
    }
    return "invalid(" + std::to_string(dir) + ")";
}


static
void
dump_sdl_event(const SDL_MouseWheelEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", which=%s, direction=%s,"
           " x=%d, y=%d, preciseX=%f, preciseY=%f, mouseX=%d, mouseY=%d\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           get_mouse_name(e.which).data(),
           wheel_dir_to_string(e.direction).data(),
           e.x,
           e.y,
           e.preciseX,
           e.preciseY,
           e.mouseX,
           e.mouseY);
}


static
void
dump_sdl_event(const SDL_QuitEvent& e)
{
    printf("%s\n", event_type_to_string(e.type).data());
}


static
int
get_touch_device_index(SDL_TouchID id)
{
    int n = SDL_GetNumTouchDevices();
    for (int i = 0; i < n; ++i)
        if (id == SDL_GetTouchDevice(i))
            return i;
    return -1;
}


static
const char*
touch_device_name_from_id(SDL_TouchID id)
{
    int idx = get_touch_device_index(id);
    if (idx == -1)
        return "<NULL>";
    auto result = SDL_GetTouchName(idx);
    if (!result)
        return "<NULL>";
    return result;
}


static
void
dump_sdl_event(const SDL_TouchFingerEvent& e)
{
    printf("%s: touchID=%" SDL_PRIs64 ", name=\"%s\", fingerID=%" SDL_PRIs64 ","
           " x=%f, y=%f, dx=%f, dy=%f, pressure=%f, windowID=%" SDL_PRIu32 "\n",
           event_type_to_string(e.type).data(),
           e.touchId,
           touch_device_name_from_id(e.touchId),
           e.fingerId,
           e.x,
           e.y,
           e.dx,
           e.dy,
           e.pressure,
           e.windowID);
}


static
std::string
window_event_id_to_string(Uint32 id)
{
    switch (id) {
        case SDL_WINDOWEVENT_NONE:
            return "SDL_WINDOWEVENT_NONE";
        case SDL_WINDOWEVENT_SHOWN:
            return "SDL_WINDOWEVENT_SHOWN";
        case SDL_WINDOWEVENT_HIDDEN:
            return "SDL_WINDOWEVENT_HIDDEN";
        case SDL_WINDOWEVENT_EXPOSED:
            return "SDL_WINDOWEVENT_EXPOSED";
        case SDL_WINDOWEVENT_MOVED:
            return "SDL_WINDOWEVENT_MOVED";
        case SDL_WINDOWEVENT_RESIZED:
            return "SDL_WINDOWEVENT_RESIZED";
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            return "SDL_WINDOWEVENT_SIZE_CHANGED";
        case SDL_WINDOWEVENT_MINIMIZED:
            return "SDL_WINDOWEVENT_MINIMIZED";
        case SDL_WINDOWEVENT_MAXIMIZED:
            return "SDL_WINDOWEVENT_MAXIMIZED";
        case SDL_WINDOWEVENT_RESTORED:
            return "SDL_WINDOWEVENT_RESTORED";
        case SDL_WINDOWEVENT_ENTER:
            return "SDL_WINDOWEVENT_ENTER";
        case SDL_WINDOWEVENT_LEAVE:
            return "SDL_WINDOWEVENT_LEAVE";
        case SDL_WINDOWEVENT_FOCUS_GAINED:
            return "SDL_WINDOWEVENT_FOCUS_GAINED";
        case SDL_WINDOWEVENT_FOCUS_LOST:
            return "SDL_WINDOWEVENT_FOCUS_LOST";
        case SDL_WINDOWEVENT_CLOSE:
            return "SDL_WINDOWEVENT_CLOSE";
        case SDL_WINDOWEVENT_TAKE_FOCUS:
            return "SDL_WINDOWEVENT_TAKE_FOCUS";
        case SDL_WINDOWEVENT_HIT_TEST:
            return "SDL_WINDOWEVENT_HIT_TEST";
        case SDL_WINDOWEVENT_ICCPROF_CHANGED:
            return "SDL_WINDOWEVENT_ICCPROF_CHANGED";
        case SDL_WINDOWEVENT_DISPLAY_CHANGED:
            return "SDL_WINDOWEVENT_DISPLAY_CHANGED";
    }
    return "invalid(" + std::to_string(id) + ")";
}


static
void
dump_sdl_event(const SDL_WindowEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", event=%s\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           window_event_id_to_string(e.event).data());
}


static
std::string
append_list(const std::string& a,
            const std::string& b,
            const std::string& separator = "+")
{
    if (a.empty())
        return b;
    if (b.empty())
        return a;
    return a + separator + b;
}


static
std::string
keysym_mod_to_string(Uint16 modifier)
{
    std::string result;
    if (modifier & KMOD_LSHIFT)
        result = append_list(result, "LSHIFT");
    if (modifier & KMOD_RSHIFT)
        result = append_list(result, "RSHIFT");
    if (modifier & KMOD_LCTRL)
        result = append_list(result, "LCTRL");
    if (modifier & KMOD_RCTRL)
        result = append_list(result, "RCTRL");
    if (modifier & KMOD_LALT)
        result = append_list(result, "LALT");
    if (modifier & KMOD_RALT)
        result = append_list(result, "RALT");
    if (modifier & KMOD_LGUI)
        result = append_list(result, "LGUI");
    if (modifier & KMOD_RGUI)
        result = append_list(result, "RGUI");
    if (modifier & KMOD_NUM)
        result = append_list(result, "NUM");
    if (modifier & KMOD_CAPS)
        result = append_list(result, "CAPS");
    if (modifier & KMOD_MODE)
        result = append_list(result, "MODE");
    if (modifier & KMOD_SCROLL)
        result = append_list(result, "SCROLL");
    return result;
}


static
std::string
keysym_to_string(const SDL_Keysym& keysym)
{
    std::string result;
    result += "scancode=\""s + SDL_GetScancodeName(keysym.scancode) + "\""s;
    result += ", sym=\""s + SDL_GetKeyName(keysym.sym) + "\""s;
    result += ", mod="s + keysym_mod_to_string(keysym.mod);
    return "{" + result + "}";
}


static
void
dump_sdl_event(const SDL_KeyboardEvent&e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", state=%s, repeat=%u, keysym=%s\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           button_state_to_string(e.state).data(),
           unsigned{e.repeat},
           keysym_to_string(e.keysym).data());
}


static
void
dump_sdl_event(const SDL_TextInputEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", text=\"%s\"\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           e.text);
}


static
void
dump_sdl_event(const SDL_UserEvent& e)
{
    printf("%s: windowID=%" SDL_PRIu32 ", code=%" SDL_PRIs32 "\n",
           event_type_to_string(e.type).data(),
           e.windowID,
           e.code);
}


static
void
dump_sdl_event_generic(const SDL_Event& e)
{
    printf("%s\n", event_type_to_string(e.type).data());
}


void
dump_sdl_event(const SDL_Event& e)
{
    switch (e.type) {

        case SDL_QUIT:
            dump_sdl_event(e.quit);
            break;

        case SDL_DISPLAYEVENT:
            dump_sdl_event(e.display);
            break;

        case SDL_WINDOWEVENT:
            dump_sdl_event(e.window);
            break;

        case SDL_MOUSEMOTION:
            dump_sdl_event(e.motion);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            dump_sdl_event(e.button);
            break;

        case SDL_MOUSEWHEEL:
            dump_sdl_event(e.wheel);
            break;

        case SDL_JOYAXISMOTION:
            dump_sdl_event(e.jaxis);
            break;

        case SDL_JOYBALLMOTION:
            dump_sdl_event(e.jball);
            break;

        case SDL_JOYHATMOTION:
            dump_sdl_event(e.jhat);
            break;

        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            dump_sdl_event(e.jbutton);
            break;

        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED:
            dump_sdl_event(e.jdevice);
            break;

        case SDL_JOYBATTERYUPDATED:
            dump_sdl_event(e.jbattery);
            break;

        case SDL_CONTROLLERAXISMOTION:
            dump_sdl_event(e.caxis);
            break;

        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP:
            dump_sdl_event(e.cbutton);
            break;

        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED:
            dump_sdl_event(e.cdevice);
            break;

        case SDL_CONTROLLERTOUCHPADDOWN:
        case SDL_CONTROLLERTOUCHPADMOTION:
        case SDL_CONTROLLERTOUCHPADUP:
            dump_sdl_event(e.ctouchpad);
            break;

        case SDL_CONTROLLERSENSORUPDATE:
            dump_sdl_event(e.csensor);
            break;

        case SDL_FINGERDOWN:
        case SDL_FINGERUP:
        case SDL_FINGERMOTION:
            dump_sdl_event(e.tfinger);
            break;

        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED:
            dump_sdl_event(e.adevice);
            break;

        case SDL_KEYDOWN:
        case SDL_KEYUP:
            dump_sdl_event(e.key);
            break;

        case SDL_TEXTINPUT:
            dump_sdl_event(e.text);
            break;

        case SDL_APP_DIDENTERBACKGROUND:
        case SDL_APP_DIDENTERFOREGROUND:
        case SDL_APP_LOWMEMORY:
        case SDL_APP_TERMINATING:
        case SDL_APP_WILLENTERBACKGROUND:
        case SDL_APP_WILLENTERFOREGROUND:
        case SDL_CLIPBOARDUPDATE:
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD:
        case SDL_DROPBEGIN:
        case SDL_DROPCOMPLETE:
        case SDL_DROPFILE:
        case SDL_DROPTEXT:
        case SDL_KEYMAPCHANGED:
        case SDL_LOCALECHANGED:
        case SDL_MULTIGESTURE:
        case SDL_POLLSENTINEL:
        case SDL_RENDER_DEVICE_RESET:
        case SDL_RENDER_TARGETS_RESET:
        case SDL_SENSORUPDATE:
        case SDL_SYSWMEVENT:
        case SDL_TEXTEDITING:
        case SDL_TEXTEDITING_EXT:
            dump_sdl_event_generic(e);
            break;

        default:
            if (e.type >= SDL_USEREVENT && e.type <= SDL_LASTEVENT)
                dump_sdl_event(e.user);
            else
                printf("ERROR: invalid event type: %" SDL_PRIu32 "\n", e.type);

    }
}
