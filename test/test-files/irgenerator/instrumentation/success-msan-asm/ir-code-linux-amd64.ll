; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Direct: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [14 x i8] c"Indirect: %d\0A\00", align 4
@llvm.used = appending global [1 x ptr] [ptr @msan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 0, ptr @msan.module_ctor, ptr null }]
@__msan_retval_tls = external thread_local(initialexec) global [100 x i64]
@__msan_retval_origin_tls = external thread_local(initialexec) global i32
@__msan_param_tls = external thread_local(initialexec) global [100 x i64]
@__msan_param_origin_tls = external thread_local(initialexec) global [200 x i32]
@__msan_va_arg_tls = external thread_local(initialexec) global [100 x i64]
@__msan_va_arg_origin_tls = external thread_local(initialexec) global [200 x i32]
@__msan_va_arg_overflow_size_tls = external thread_local(initialexec) global i64

; Function Attrs: nounwind
declare noundef ptr @malloc(i64 noundef) #0

; Function Attrs: nounwind
declare void @free(ptr noundef) #0

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_memory uwtable
define noundef i32 @main() #1 {
  call void @llvm.donothing()
  %direct = alloca i64, align 8
  %memory = alloca ptr, align 8
  %indirect = alloca ptr, align 8
  %1 = call i64 asm sideeffect "movq $$42, $0", "=r,~{dirflag},~{fpsr},~{flags}"()
  call void @llvm.lifetime.start.p0(ptr %direct)
  %2 = ptrtoint ptr %direct to i64
  %3 = xor i64 %2, 87960930222080
  %4 = inttoptr i64 %3 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %4, i8 -1, i64 8, i1 false)
  %5 = ptrtoint ptr %direct to i64
  %6 = xor i64 %5, 87960930222080
  %7 = inttoptr i64 %6 to ptr
  store i64 0, ptr %7, align 8
  store i64 %1, ptr %direct, align 8
  %8 = load i64, ptr %direct, align 8
  %9 = ptrtoint ptr %direct to i64
  %10 = xor i64 %9, 87960930222080
  %11 = inttoptr i64 %10 to ptr
  %_msld = load i64, ptr %11, align 8
  store i64 %_msld, ptr getelementptr (i8, ptr @__msan_va_arg_tls, i64 8), align 8
  store i64 0, ptr @__msan_va_arg_overflow_size_tls, align 8
  %_mscmp = icmp ne i64 %_msld, 0
  br i1 %_mscmp, label %12, label %13, !prof !6

12:                                               ; preds = %0
  call void @__msan_warning_noreturn() #7
  unreachable

13:                                               ; preds = %0
  %14 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %8)
  %15 = call noundef ptr @malloc(i64 noundef 16)
  call void @llvm.lifetime.start.p0(ptr %memory)
  %16 = ptrtoint ptr %memory to i64
  %17 = xor i64 %16, 87960930222080
  %18 = inttoptr i64 %17 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %18, i8 -1, i64 8, i1 false)
  %19 = ptrtoint ptr %memory to i64
  %20 = xor i64 %19, 87960930222080
  %21 = inttoptr i64 %20 to ptr
  store i64 0, ptr %21, align 8
  store ptr %15, ptr %memory, align 8
  call void @llvm.lifetime.start.p0(ptr %indirect)
  %22 = ptrtoint ptr %indirect to i64
  %23 = xor i64 %22, 87960930222080
  %24 = inttoptr i64 %23 to ptr
  call void @llvm.memset.p0.i64(ptr align 8 %24, i8 -1, i64 8, i1 false)
  %25 = load ptr, ptr %memory, align 8
  %26 = ptrtoint ptr %memory to i64
  %27 = xor i64 %26, 87960930222080
  %28 = inttoptr i64 %27 to ptr
  %_msld1 = load i64, ptr %28, align 8
  %29 = ptrtoint ptr %indirect to i64
  %30 = xor i64 %29, 87960930222080
  %31 = inttoptr i64 %30 to ptr
  store i64 %_msld1, ptr %31, align 8
  store ptr %25, ptr %indirect, align 8
  %32 = load ptr, ptr %indirect, align 8
  %33 = ptrtoint ptr %indirect to i64
  %34 = xor i64 %33, 87960930222080
  %35 = inttoptr i64 %34 to ptr
  %_msld2 = load i64, ptr %35, align 8
  %36 = ptrtoint ptr %32 to i64
  %37 = xor i64 %36, 87960930222080
  %38 = inttoptr i64 %37 to ptr
  store i64 0, ptr %38, align 1
  %_mscmp8 = icmp ne i64 %_msld2, 0
  br i1 %_mscmp8, label %39, label %40, !prof !6

39:                                               ; preds = %13
  call void @__msan_warning_noreturn() #7
  unreachable

40:                                               ; preds = %13
  call void asm sideeffect "movq $$7, $0", "=*m,~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %32)
  %41 = load ptr, ptr %indirect, align 8
  %42 = ptrtoint ptr %indirect to i64
  %43 = xor i64 %42, 87960930222080
  %44 = inttoptr i64 %43 to ptr
  %_msld3 = load i64, ptr %44, align 8
  %_mscmp9 = icmp ne i64 %_msld3, 0
  br i1 %_mscmp9, label %45, label %46, !prof !6

45:                                               ; preds = %40
  call void @__msan_warning_noreturn() #7
  unreachable

46:                                               ; preds = %40
  %47 = load i64, ptr %41, align 8
  %48 = ptrtoint ptr %41 to i64
  %49 = xor i64 %48, 87960930222080
  %50 = inttoptr i64 %49 to ptr
  %_msld4 = load i64, ptr %50, align 8
  %51 = xor i64 %47, 7
  %52 = or i64 %_msld4, 0
  %53 = icmp ne i64 %52, 0
  %54 = xor i64 %52, -1
  %55 = and i64 %54, %51
  %56 = icmp eq i64 %55, 0
  %_msprop_icmp = and i1 %53, %56
  %57 = icmp eq i64 %47, 7
  br i1 %_msprop_icmp, label %58, label %59, !prof !6

58:                                               ; preds = %46
  call void @__msan_warning_noreturn() #7
  unreachable

59:                                               ; preds = %46
  br i1 %57, label %if.then.L17, label %if.exit.L17

if.then.L17:                                      ; preds = %59
  %60 = load ptr, ptr %indirect, align 8
  %61 = ptrtoint ptr %indirect to i64
  %62 = xor i64 %61, 87960930222080
  %63 = inttoptr i64 %62 to ptr
  %_msld5 = load i64, ptr %63, align 8
  %_mscmp10 = icmp ne i64 %_msld5, 0
  br i1 %_mscmp10, label %64, label %65, !prof !6

64:                                               ; preds = %if.then.L17
  call void @__msan_warning_noreturn() #7
  unreachable

65:                                               ; preds = %if.then.L17
  %66 = load i64, ptr %60, align 8
  %67 = ptrtoint ptr %60 to i64
  %68 = xor i64 %67, 87960930222080
  %69 = inttoptr i64 %68 to ptr
  %_msld6 = load i64, ptr %69, align 8
  store i64 %_msld6, ptr getelementptr (i8, ptr @__msan_va_arg_tls, i64 8), align 8
  store i64 0, ptr @__msan_va_arg_overflow_size_tls, align 8
  %_mscmp11 = icmp ne i64 %_msld6, 0
  br i1 %_mscmp11, label %70, label %71, !prof !6

70:                                               ; preds = %65
  call void @__msan_warning_noreturn() #7
  unreachable

71:                                               ; preds = %65
  %72 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef %66)
  br label %if.exit.L17

if.exit.L17:                                      ; preds = %71, %59
  %73 = load ptr, ptr %memory, align 8
  %74 = ptrtoint ptr %memory to i64
  %75 = xor i64 %74, 87960930222080
  %76 = inttoptr i64 %75 to ptr
  %_msld7 = load i64, ptr %76, align 8
  %_mscmp12 = icmp ne i64 %_msld7, 0
  br i1 %_mscmp12, label %77, label %78, !prof !6

77:                                               ; preds = %if.exit.L17
  call void @__msan_warning_noreturn() #7
  unreachable

78:                                               ; preds = %if.exit.L17
  call void @free(ptr noundef align 1 %73)
  call void @llvm.lifetime.end.p0(ptr %direct)
  call void @llvm.lifetime.end.p0(ptr %indirect)
  call void @llvm.lifetime.end.p0(ptr %memory)
  ret i32 0
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #2

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #2

declare void @__msan_init()

; Function Attrs: nounwind uwtable
define internal void @msan.module_ctor() #4 {
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
declare void @llvm.donothing() #5

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr writeonly captures(none), i8, i64, i1 immarg) #6

attributes #0 = { nounwind }
attributes #1 = { mustprogress noinline norecurse nounwind optnone sanitize_memory uwtable }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nofree nounwind }
attributes #4 = { nounwind uwtable }
attributes #5 = { nocallback nofree nosync nounwind willreturn memory(none) }
attributes #6 = { nocallback nofree nosync nounwind willreturn memory(argmem: write) }
attributes #7 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{i32 4, !"nosanitize_memory", i32 1}
!5 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!6 = !{!"branch_weights", i32 1, i32 1048575}
