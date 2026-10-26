; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@ADD_TEMPLATE = internal constant ptr @anon.string.0
@anon.string.0 = private unnamed_addr constant [12 x i8] c"addl $2, $0\00", align 4
@ADD_CONSTRAINTS = internal constant ptr @anon.string.1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"=r,0,r\00", align 4
@printf.str.0 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [13 x i8] c"Doubled: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [19 x i8] c"Parenthesized: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [14 x i8] c"Imported: %d\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %sum = alloca i32, align 4
  %doubled = alloca i32, align 4
  %parenthesized = alloca i32, align 4
  %loaded = alloca i64, align 8
  store i32 40, ptr %a, align 4
  store i32 2, ptr %b, align 4
  %1 = load i32, ptr %a, align 4
  %2 = load i32, ptr %b, align 4
  %3 = call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %1, i32 %2)
  store i32 %3, ptr %sum, align 4
  %4 = load i32, ptr %sum, align 4
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4)
  %6 = load i32, ptr %sum, align 4
  %7 = load i32, ptr %sum, align 4
  %8 = call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %6, i32 %7)
  store i32 %8, ptr %doubled, align 4
  %9 = load i32, ptr %doubled, align 4
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %9)
  %11 = load i32, ptr %a, align 4
  %12 = load i32, ptr %b, align 4
  %13 = call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %11, i32 %12)
  store i32 %13, ptr %parenthesized, align 4
  %14 = load i32, ptr %parenthesized, align 4
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %14)
  %16 = call i64 asm sideeffect "movq $1, $0", "=r,r,~{dirflag},~{fpsr},~{flags}"(i64 42)
  store i64 %16, ptr %loaded, align 8
  %17 = load i64, ptr %loaded, align 8
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i64 noundef %17)
  call void asm sideeffect "mfence", "~{memory},~{dirflag},~{fpsr},~{flags}"()
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
