#ifndef FCPP_CFCPP_H
#define FCPP_CFCPP_H

#include <stddef.h>
#include <stdint.h>

#include <CFCPPExport.h>

#ifdef __cplusplus
#define CFCPP_API extern "C" CFCPP_EXPORT
#define CFCPP_NOEXCEPT noexcept
#else
#define CFCPP_API extern CFCPP_EXPORT
#define CFCPP_NOEXCEPT
#endif

typedef struct fcpp_fc* fcpp_fc_t;
typedef struct fcpp_ines* fcpp_ines_t;
typedef struct fcpp_snapshot* fcpp_snapshot_t;

enum fcpp_joypad_type {
    FCPP_JOYPAD_STANDARD
};

enum fcpp_standard_buttons {
    FCPP_STD_A = 0,
    FCPP_STD_B = 1,
    FCPP_STD_SELECT = 2,
    FCPP_STD_START = 3,
    FCPP_STD_UP = 4,
    FCPP_STD_DOWN = 5,
    FCPP_STD_LEFT = 6,
    FCPP_STD_RIGHT = 7
};

struct fcpp_input_scanner {
    uint8_t(*scan)(void* data) CFCPP_NOEXCEPT;
    enum fcpp_joypad_type(*get_joypad_type)(void* data) CFCPP_NOEXCEPT;
    void* data;
};

struct fcpp_frame_buffer {
    void(*set_pixel)(int x, int y, uint32_t color, void* data) CFCPP_NOEXCEPT;
    void(*completed_signal)(void* data) CFCPP_NOEXCEPT;
    const uint32_t* (*get_palette_table)(void* data) CFCPP_NOEXCEPT;
    void* data;
};

struct fcpp_sample_buffer
{
    void(*send_sample)(double sample, void* data) CFCPP_NOEXCEPT;
    int(*get_sample_rate)(void* data) CFCPP_NOEXCEPT;
    void* data;
};

CFCPP_API fcpp_fc_t fcpp_fc_create(void) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_destroy(fcpp_fc_t fc) CFCPP_NOEXCEPT;
CFCPP_API int fcpp_fc_insert_cartridge_from_file(fcpp_fc_t fc, const char* path) CFCPP_NOEXCEPT;
CFCPP_API int fcpp_fc_insert_cartridge_from_ines(fcpp_fc_t fc, fcpp_ines_t ines) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_connect_input_scanner(fcpp_fc_t fc, int index, const struct fcpp_input_scanner* input_scanner) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_connect_frame_buffer(fcpp_fc_t fc, const struct fcpp_frame_buffer* frame_buffer) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_connect_sample_buffer(fcpp_fc_t fc, const struct fcpp_sample_buffer* sample_buffer) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_set_frame_rate(fcpp_fc_t fc, double fps) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_set_sprite_limit(fcpp_fc_t fc, int limit) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_power_on(fcpp_fc_t fc) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_reset(fcpp_fc_t fc) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_save(fcpp_fc_t fc, fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_load(fcpp_fc_t fc, fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_fc_exec(fcpp_fc_t fc) CFCPP_NOEXCEPT;

CFCPP_API fcpp_ines_t fcpp_ines_create(void) CFCPP_NOEXCEPT;
CFCPP_API fcpp_ines_t fcpp_ines_create_from(fcpp_ines_t other) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_ines_destroy(fcpp_ines_t ines) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_ines_load_from_file(fcpp_ines_t ines, const char* path) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_ines_load_from_buffer(fcpp_ines_t ines, const uint8_t* buffer, size_t size) CFCPP_NOEXCEPT;
CFCPP_API uint8_t fcpp_ines_get_prg_banks(fcpp_ines_t ines) CFCPP_NOEXCEPT;
CFCPP_API uint8_t fcpp_ines_get_chr_banks(fcpp_ines_t ines) CFCPP_NOEXCEPT;
CFCPP_API uint8_t fcpp_ines_get_mapper_type(fcpp_ines_t ines) CFCPP_NOEXCEPT;
CFCPP_API uint8_t fcpp_ines_read_prg(fcpp_ines_t ines, uint32_t addr) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_ines_write_prg(fcpp_ines_t ines, uint32_t addr, uint8_t data) CFCPP_NOEXCEPT;
CFCPP_API uint8_t fcpp_ines_read_chr(fcpp_ines_t ines, uint32_t addr) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_ines_write_chr(fcpp_ines_t ines, uint32_t addr, uint8_t data) CFCPP_NOEXCEPT;

CFCPP_API fcpp_snapshot_t fcpp_snapshot_create(void) CFCPP_NOEXCEPT;
CFCPP_API fcpp_snapshot_t fcpp_snapshot_create_from(fcpp_snapshot_t other) CFCPP_NOEXCEPT;
CFCPP_API void fcpp_snapshot_destroy(fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;
CFCPP_API size_t fcpp_snapshot_size(fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;
CFCPP_API size_t fcpp_snapshot_capacity(fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;
CFCPP_API uint8_t* fcpp_snapshot_data(fcpp_snapshot_t snapshot) CFCPP_NOEXCEPT;

#endif
