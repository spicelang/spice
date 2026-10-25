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

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z9addViaAsmRKii(ptr noundef %0, i32 noundef %1) #0 {
  %a = alloca ptr, align 8
  %b = alloca i32, align 4
  store ptr %0, ptr %a, align 8
  store i32 %1, ptr %b, align 4
  %3 = load ptr, ptr %a, align 8
  %4 = load i32, ptr %3, align 4
  %5 = load i32, ptr %b, align 4
  %6 = call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %4, i32 %5)
  ret i32 %6
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #1 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %sum = alloca i32, align 4
  %value = alloca i64, align 8
  %source = alloca i64, align 8
  %loaded = alloca i64, align 8
  %flag = alloca i1, align 1
  %color = alloca i32, align 4
  %ptr = alloca ptr, align 8
  %ptrCopy = alloca ptr, align 8
  %d = alloca double, align 8
  store i32 40, ptr %a, align 4
  store i32 2, ptr %b, align 4
  %1 = load i32, ptr %a, align 4
  %2 = load i32, ptr %b, align 4
  %3 = call i32 asm sideeffect "addl $2, $0", "=r,0,r,~{dirflag},~{fpsr},~{flags}"(i32 %1, i32 %2)
  store i32 %3, ptr %sum, align 4
  %4 = load i32, ptr %sum, align 4
  %5 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %4)
  call void asm sideeffect "mfence", "~{memory},~{dirflag},~{fpsr},~{flags}"()
  call void asm sideeffect "nop\0A\09nop", "~{dirflag},~{fpsr},~{flags}"()
  store i64 0, ptr %value, align 8
  call void asm sideeffect "movq $$42, $0", "=*m,~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %value)
  %6 = load i64, ptr %value, align 8
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef %6)
  store i64 7, ptr %source, align 8
  %8 = call i64 asm sideeffect "movq $1, $0", "=r,*m,~{dirflag},~{fpsr},~{flags}"(ptr elementtype(i64) %source)
  store i64 %8, ptr %loaded, align 8
  %9 = load i64, ptr %loaded, align 8
  %10 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 noundef %9)
  %11 = call i8 asm sideeffect "movb $1, $0", "=r,r,~{dirflag},~{fpsr},~{flags}"(i8 1)
  %12 = trunc i8 %11 to i1
  store i1 %12, ptr %flag, align 1
  %13 = load i1, ptr %flag, align 1
  %14 = zext i1 %13 to i32
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %14)
  %16 = call i32 asm sideeffect "movl $1, $0", "=r,r,~{dirflag},~{fpsr},~{flags}"(i32 2)
  store i32 %16, ptr %color, align 4
  %17 = load i32, ptr %color, align 4
  %18 = icmp eq i32 %17, 2
  br i1 %18, label %land.1.L37C30, label %land.exit.L37C30

land.1.L37C30:                                    ; preds = %0
  %19 = load i32, ptr %color, align 4
  %20 = icmp ne i32 %19, 0
  br i1 %20, label %land.2.L37C30, label %land.exit.L37C30

land.2.L37C30:                                    ; preds = %land.1.L37C30
  %21 = load i32, ptr %color, align 4
  %22 = icmp ne i32 %21, 1
  br label %land.exit.L37C30

land.exit.L37C30:                                 ; preds = %land.2.L37C30, %land.1.L37C30, %0
  %land_phi = phi i1 [ %18, %0 ], [ %20, %land.1.L37C30 ], [ %22, %land.2.L37C30 ]
  %23 = zext i1 %land_phi to i32
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %23)
  store ptr %a, ptr %ptr, align 8
  %25 = load ptr, ptr %ptr, align 8
  %26 = call ptr asm sideeffect "movq $1, $0", "=&r,{rdi},~{dirflag},~{fpsr},~{flags}"(ptr %25)
  store ptr %26, ptr %ptrCopy, align 8
  %27 = load ptr, ptr %ptrCopy, align 8
  %28 = load ptr, ptr %ptr, align 8
  %29 = icmp eq ptr %28, %27
  %30 = zext i1 %29 to i32
  %31 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i32 noundef %30)
  %32 = call double asm sideeffect "addsd $2, $0", "=x,0,x,~{dirflag},~{fpsr},~{flags}"(double 1.500000e+00, double 2.250000e+00)
  store double %32, ptr %d, align 8
  %33 = load double, ptr %d, align 8
  %34 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, double noundef %33)
  %35 = load i32, ptr %b, align 4
  %36 = call noundef i32 @_Z9addViaAsmRKii(ptr noundef %a, i32 noundef %35)
  %37 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %36)
  call void asm sideeffect "# ${:comment} ${:uid}", "~{dirflag},~{fpsr},~{flags}"()
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
