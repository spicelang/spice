; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@printf.str.0 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [21 x i8] c"Indirect output: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [20 x i8] c"Indirect input: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [10 x i8] c"Bool: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [10 x i8] c"Enum: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [13 x i8] c"Pointer: %d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [12 x i8] c"Double: %f\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [15 x i8] c"Reference: %d\0A\00", align 4

; Function Attrs: noinline norecurse nounwind uwtable
define noundef i32 @main() local_unnamed_addr #0 {
land.exit.L37C30:
  %a = alloca i32, align 4
  %value = alloca i64, align 8
  %source = alloca i64, align 8
  store i32 40, ptr %a, align 4
  %0 = tail call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 40, i32 2) #2
  %1 = tail call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i32 noundef %0)
  tail call void asm sideeffect "mfence", "~{memory},~{dirflag},~{fpsr},~{flags}"() #2
  tail call void asm sideeffect "nop\0A\09nop", "~{dirflag},~{fpsr},~{flags}"() #2
  store i64 0, ptr %value, align 8
  call void asm sideeffect "movq $$42, $0", "=*m,~{dirflag},~{fpsr},~{flags}"(ptr nonnull elementtype(i64) %value) #2
  %2 = load i64, ptr %value, align 8
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i64 noundef %2)
  store i64 7, ptr %source, align 8
  %4 = call i64 asm sideeffect "movq $1, $0", "=r,*m,~{dirflag},~{fpsr},~{flags}"(ptr nonnull elementtype(i64) %source) #2
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.2, i64 noundef %4)
  %6 = call i8 asm sideeffect "movb $1, $0", "=r,r,~{dirflag},~{fpsr},~{flags}"(i8 1) #2
  %7 = and i8 %6, 1
  %8 = zext nneg i8 %7 to i32
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3, i32 noundef %8)
  %10 = call i32 asm sideeffect "movl $1, $0", "=r,r,~{dirflag},~{fpsr},~{flags}"(i32 2) #2
  %11 = icmp eq i32 %10, 2
  %12 = zext i1 %11 to i32
  %13 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.4, i32 noundef %12)
  %14 = call ptr asm sideeffect "movq $1, $0", "=&r,{rdi},~{dirflag},~{fpsr},~{flags}"(ptr nonnull %a) #2
  %15 = icmp eq ptr %a, %14
  %16 = zext i1 %15 to i32
  %17 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.5, i32 noundef %16)
  %18 = call double asm sideeffect "addsd $2, $0", "=x,0,x,~{dirflag},~{fpsr},~{flags}"(double 1.500000e+00, double 2.250000e+00) #2
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.6, double noundef %18)
  %20 = load i32, ptr %a, align 4
  %21 = call noundef i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %20, i32 2) #2
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.7, i32 noundef %21)
  call void asm sideeffect "# ${:comment} ${:uid}", "~{dirflag},~{fpsr},~{flags}"() #2
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { noinline norecurse nounwind uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
