/*
 * mayhem/asan_options.c — baked ASan/LSan default options for the BAG fuzz harnesses.
 *
 * HDF5's internal allocator (H5C_protect et al.) issues large allocation probes on
 * corrupted-size fields; without allocator_may_return_null=1 ASan aborts before the
 * harness can handle a NULL return, producing 0-edge runs on any input that reaches
 * that code path.
 *
 * HDF5 and libxml2 intentionally retain some internal caches across calls; with
 * detect_leaks=1 (LeakSanitizer) ASan aborts on the first corpus seed that succeeds
 * in reaching the HDF5 parser, again producing 0-edge runs.
 *
 * Both options are set here at the binary level so Mayhem's runtime ASAN_OPTIONS
 * (abort_on_error=1, symbolize=0, …) is not replaced — __asan_default_options is
 * the weak override, not a replacement: Mayhem's env-var value wins for any key it
 * sets, and we only add the two keys Mayhem does not set.
 *
 * __lsan_default_options is the direct LSan override (belt-and-suspenders alongside
 * the detect_leaks=0 in __asan_default_options). Under ptrace (Mayhem's coverage
 * collector) LSan's attach fails, it exits non-zero before edges are recorded. Both
 * symbols are STRONG (no __attribute__((weak))) so they always win over any ASan/LSan
 * runtime weak copy, including those pulled in via sanitized shared libraries.
 */
const char *__asan_default_options(void)
{
    return "detect_leaks=0:allocator_may_return_null=1";
}

const char *__lsan_default_options(void)
{
    return "detect_leaks=0";
}
