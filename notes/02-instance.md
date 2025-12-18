First, the renderer:
    The renderer uses make_instance to make a vulkan instance, and puts the instance's deletion function onto the deletion queue. Then when the engine wraps everything up (when ~Engine() is called), it just deletes things in the opposite order (meaning newest is deleted first).
