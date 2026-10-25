; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i8:8:32-i16:16:32-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "aarch64-unknown-linux-gnu"

@printf.str.0 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [13 x i8] c"Product: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [21 x i8] c"Indirect output: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind uwtable
define noundef i32 @main() local_unnamed_addr #0 {
  %value = alloca i64, align 8
  %1 = tail call i64 asm sideeffect "add $0, $1, $2", "=r,r,r"(i64 40, i64 2) #2
  %2 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 noundef %1)
  %3 = tail call i32 asm sideeffect "mul ${0:w}, ${1:w}, ${2:w}", "=r,r,r"(i32 6, i32 7) #2
  %4 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 noundef %3)
  tail call void asm sideeffect "dmb ish", "~{memory}"() #2
  store i64 0, ptr %value, align 8
  call void asm sideeffect "mov x9, #42\0A\09str x9, $0", "=*m,~{x9}"(ptr nonnull elementtype(i64) %value) #2
  %5 = load i64, ptr %value, align 8
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i64 noundef %5)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { mustprogress noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
