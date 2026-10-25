; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

@printf.str.0 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [13 x i8] c"Product: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [21 x i8] c"Indirect output: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  %sum = alloca i64, align 8
  %product = alloca i32, align 4
  %value = alloca i64, align 8
  store i64 40, ptr %a, align 8
  store i64 2, ptr %b, align 8
  %1 = load i64, ptr %a, align 8
  %2 = load i64, ptr %b, align 8
  %3 = call i64 asm sideeffect "add $0, $1, $2", "=r,r,r"(i64 %1, i64 %2)
  store i64 %3, ptr %sum, align 8
  %4 = load i64, ptr %sum, align 8
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %4)
  %6 = call i32 asm sideeffect "mul ${0:w}, ${1:w}, ${2:w}", "=r,r,r"(i32 6, i32 7)
  store i32 %6, ptr %product, align 4
  %7 = load i32, ptr %product, align 4
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %7)
  call void asm sideeffect "dmb ish", "~{memory}"()
  store i64 0, ptr %value, align 8
  call void asm sideeffect "mov x9, #42\0A\09str x9, $0", "=*m,~{x9}"(ptr elementtype(i64) %value)
  %9 = load i64, ptr %value, align 8
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 noundef %9)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
