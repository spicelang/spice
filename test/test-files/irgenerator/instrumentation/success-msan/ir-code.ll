; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 4
@llvm.used = appending global [1 x ptr] [ptr @msan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @msan.module_ctor, ptr null }]
@__msan_retval_tls = external thread_local(initialexec) global [100 x i64]
@__msan_retval_origin_tls = external thread_local(initialexec) global i32
@__msan_param_tls = external thread_local(initialexec) global [100 x i64]
@__msan_param_origin_tls = external thread_local(initialexec) global [200 x i32]
@__msan_va_arg_tls = external thread_local(initialexec) global [100 x i64]
@__msan_va_arg_origin_tls = external thread_local(initialexec) global [200 x i32]
@__msan_va_arg_overflow_size_tls = external thread_local(initialexec) global i64

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_memory uwtable
define dso_local noundef i32 @main() #0 {
  call void @llvm.donothing()
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr %result)
  %1 = ptrtoint ptr %result to i64
  %2 = xor i64 %1, 87960930222080
  %3 = inttoptr i64 %2 to ptr
  call void @llvm.memset.p0.i64(ptr align 4 %3, i8 -1, i64 4, i1 false)
  %4 = ptrtoint ptr %result to i64
  %5 = xor i64 %4, 87960930222080
  %6 = inttoptr i64 %5 to ptr
  store i32 0, ptr %6, align 4
  store i32 0, ptr %result, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr %i)
  %7 = ptrtoint ptr %i to i64
  %8 = xor i64 %7, 87960930222080
  %9 = inttoptr i64 %8 to ptr
  call void @llvm.memset.p0.i64(ptr align 4 %9, i8 -1, i64 4, i1 false)
  %10 = load i32, ptr %i, align 4
  %11 = ptrtoint ptr %i to i64
  %12 = xor i64 %11, 87960930222080
  %13 = inttoptr i64 %12 to ptr
  %_msld = load i32, ptr %13, align 4
  %_msprop = or i32 %_msld, 0
  %14 = add nsw i32 %10, 1
  %15 = ptrtoint ptr %i to i64
  %16 = xor i64 %15, 87960930222080
  %17 = inttoptr i64 %16 to ptr
  store i32 %_msprop, ptr %17, align 4
  store i32 %14, ptr %i, align 4
  %18 = load i32, ptr %i, align 4
  %19 = ptrtoint ptr %i to i64
  %20 = xor i64 %19, 87960930222080
  %21 = inttoptr i64 %20 to ptr
  %_msld1 = load i32, ptr %21, align 4
  store i32 %_msld1, ptr inttoptr (i64 add (i64 ptrtoint (ptr @__msan_va_arg_tls to i64), i64 8) to ptr), align 8
  store i64 0, ptr @__msan_va_arg_overflow_size_tls, align 8
  %_mscmp = icmp ne i32 %_msld1, 0
  br i1 %_mscmp, label %22, label %23, !prof !6

22:                                               ; preds = %0
  call void @__msan_warning_noreturn() #6
  unreachable

23:                                               ; preds = %0
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %18)
  call void @llvm.lifetime.end.p0(i64 4, ptr %i)
  %25 = load i32, ptr %result, align 4
  %26 = ptrtoint ptr %result to i64
  %27 = xor i64 %26, 87960930222080
  %28 = inttoptr i64 %27 to ptr
  %_msld2 = load i32, ptr %28, align 4
  %_mscmp3 = icmp ne i32 %_msld2, 0
  br i1 %_mscmp3, label %29, label %30, !prof !6

29:                                               ; preds = %23
  call void @__msan_warning_noreturn() #6
  unreachable

30:                                               ; preds = %23
  ret i32 %25
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

declare void @__msan_init()

; Function Attrs: nounwind uwtable
define internal void @msan.module_ctor() #3 {
  call void @__msan_init()
  ret void
}

declare i32 @__msan_chain_origin(i32)

declare void @__msan_set_origin(ptr, i64, i32)

declare ptr @__msan_memmove(ptr, ptr, i64)

declare ptr @__msan_memcpy(ptr, ptr, i64)

declare ptr @__msan_memset(ptr, i32, i64)

declare void @__msan_instrument_asm_store(ptr, i64)

declare void @__msan_warning_noreturn()

declare void @__msan_maybe_warning_1(i8, i32)

declare void @__msan_maybe_warning_N(ptr, i64, i32)

declare void @__msan_maybe_store_origin_1(i8, ptr, i32)

declare void @__msan_maybe_warning_2(i16, i32)

declare void @__msan_maybe_store_origin_2(i16, ptr, i32)

declare void @__msan_maybe_warning_4(i32, i32)

declare void @__msan_maybe_store_origin_4(i32, ptr, i32)

declare void @__msan_maybe_warning_8(i64, i32)

declare void @__msan_maybe_store_origin_8(i64, ptr, i32)

declare void @__msan_set_alloca_origin_with_descr(ptr, i64, ptr, ptr)

declare void @__msan_set_alloca_origin_no_descr(ptr, i64, ptr)

declare void @__msan_poison_stack(ptr, i64)

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(none)
declare void @llvm.donothing() #4

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #5

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_memory uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nofree nounwind }
attributes #3 = { nounwind uwtable "frame-pointer"="all" }
attributes #4 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #5 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #6 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
!4 = !{i32 4, !"nosanitize_memory", i32 1}
!5 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!6 = !{!"branch_weights", i32 1, i32 1048575}
