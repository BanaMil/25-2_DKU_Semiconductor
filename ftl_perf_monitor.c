#include "ftl_perf_monitor.h"
#include "xil_printf.h"
#include "xil_types.h"   // u32 정의용 (필요시)

// 전역 통계 구조체 정의
FTL_STATS g_ftl_stats;

// 내부 헬퍼: 64비트 값을 hi/lo 32비트로 출력 (디버깅용)
static void PrintU64Hex(const char* name, unsigned long long value)
{
    u32 hi = (u32)(value >> 32);
    u32 lo = (u32)(value & 0xFFFFFFFF);

    xil_printf("%s : 0x%08x%08x\r\n", name, hi, lo);
}

// 통계 구조체 초기화
void InitPerfStats(void)
{
    g_ftl_stats.cnt_host_req_bytes     = 0;
    g_ftl_stats.cnt_host_write_slices  = 0;
    g_ftl_stats.cnt_nand_program_total = 0;

    g_ftl_stats.cnt_gc_trigger         = 0;
    g_ftl_stats.cnt_gc_valid_copy      = 0;
    g_ftl_stats.cnt_erase_total        = 0;

    g_ftl_stats.last_req_time = 0;
    g_ftl_stats.needs_flush   = 0;
}

// 통계 출력 (중간/최종 공용)
void PrintPerfStats(void)
{
    xil_printf("\r\n========== [FTL PERF STATS] ==========\r\n");

    // 1) 기본 카운터 출력 (hex 형태, overflow 걱정 없음)
    PrintU64Hex("Host Req Bytes      ", g_ftl_stats.cnt_host_req_bytes);
    PrintU64Hex("Host Write Slices   ", g_ftl_stats.cnt_host_write_slices);
    PrintU64Hex("NAND Program Total  ", g_ftl_stats.cnt_nand_program_total);

    PrintU64Hex("GC Trigger Count    ", g_ftl_stats.cnt_gc_trigger);
    PrintU64Hex("GC Valid Copy Count ", g_ftl_stats.cnt_gc_valid_copy);
    PrintU64Hex("Erase Total         ", g_ftl_stats.cnt_erase_total);

    // 2) WAF 계산 (host write가 0이면 계산 불가)
    if (g_ftl_stats.cnt_host_write_slices > 0) {
        unsigned long long waf100 =
            (g_ftl_stats.cnt_nand_program_total * 100ULL) /
            g_ftl_stats.cnt_host_write_slices;

        u32 wafInt  = (u32)(waf100 / 100ULL);
        u32 wafFrac = (u32)(waf100 % 100ULL);

        xil_printf("WAF                 : %u.%02u\r\n", wafInt, wafFrac);
    } else {
        xil_printf("WAF                 : N/A (host write = 0)\r\n");
    }

    xil_printf("======================================\r\n");
}
