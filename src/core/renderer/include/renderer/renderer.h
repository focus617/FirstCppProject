#pragma once

// For use by user of libcore.so
#include "core.h"
#include "renderer/buffers/AVertex_buffer.hpp"
#include "renderer/buffers/buffer_layout.hpp"
#include "renderer/buffers/AIndex_buffer.hpp"
#include "renderer/buffers/AVertex_array.hpp"

#include "renderer/buffers/AFrame_buffer.hpp"

#include "renderer/resources/shader/AShader.hpp"
#include "renderer/resources/material/ATexture.hpp"
#include "renderer/resources/material/ATexture2D.hpp"
#include "renderer/resources/material/subtexture2D.hpp"

#include "renderer/renderer/renderer.hpp"
#include "renderer/renderer/renderer2D.hpp"
#include "renderer/renderer/render_command.hpp"

#include "renderer/scene/camera/orthographic_camera.hpp"
#include "renderer/scene/camera/orthographic_camera_controller.hpp"

#include "window/core/layer.hpp"
#include "window/core/time_step.hpp"
#include "window/inputs/input_manager.hpp"
#include "window/inputs/key_codes.h"
#include "window/inputs/key_state.h"
#include "window/inputs/mouse_codes.h"
#include "window/inputs/mouse_state.h"

#include "tools/event/event.hpp"
#include "tools/event/key_event.hpp"
#include "tools/event/application_event.hpp"
#include "tools/event/mouse_event.hpp"
#include "tools/event/event_dispatcher.hpp"