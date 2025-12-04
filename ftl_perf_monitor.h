#ifndef FTL_PERF_MONITOR_H_
#define FTL_PERF_MONITOR_H_

#include "xtime_l.h"     // XTime (시간 측정용)

// FTL 전체에서 공유하는 성능/통계 구조체
typedef struct {
    // [1] WAF & 성능 지표
    unsigned long long cnt_host_req_bytes;     // 호스트 요청 총 바이트 수 (fio 검증용)
    unsigned long long cnt_host_write_slices;  // 호스트 쓰기 요청 (Slice 단위, WAF 분모)
    unsigned long long cnt_nand_program_total; // 실제 NAND 프로그램 횟수 (Page 단위, WAF 분자)

    // [2] GC/WL 효율성 지표
    unsigned long long cnt_gc_trigger;         // GC 트리거 횟수
    unsigned long long cnt_gc_valid_copy;      // GC/WL 유효 페이지 복사 횟수
    unsigned long long cnt_erase_total;        // 전체 블록 소거 횟수

    // [3] 출력/Idle 제어
    XTime last_req_time;   // 마지막 호스트 요청 처리 시각
    int   needs_flush;     // 아직 출력되지 않은 통계가 있는지 여부 (1 = 있음)
} FTL_STATS;

// 전역 통계 변수
extern FTL_STATS g_ftl_stats;

// 초기화 및 출력 함수
void InitPerfStats(void);
void PrintPerfStats(void);

#endif  // FTL_PERF_MONITOR_H_
