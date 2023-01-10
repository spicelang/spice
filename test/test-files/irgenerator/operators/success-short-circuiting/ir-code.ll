; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [15 x i8] c"Function True\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Function False\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [30 x i8] c"Logical and evaluated to: %d\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [29 x i8] c"Logical or evaluated to: %d\0A\00", align 1

define private i1 @_f__void__bool__functionTrue() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i1 true
}

declare i32 @printf(ptr noundef, ...)

define private i1 @_f__void__bool__functionFalse() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret i1 false
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_f__void__bool__functionFalse()
  br i1 %1, label %land.1.L13, label %land.exit.L13

land.1.L13:                                       ; preds = %0
  %2 = call i1 @_f__void__bool__functionTrue()
  br label %land.exit.L13

land.exit.L13:                                    ; preds = %land.1.L13, %0
  %land_phi = phi i1 [ %1, %0 ], [ %2, %land.1.L13 ]
  %3 = zext i1 %land_phi to i32
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %3)
  %5 = call i1 @_f__void__bool__functionTrue()
  br i1 %5, label %lor.exit.L16, label %lor.1.L16

lor.1.L16:                                        ; preds = %land.exit.L13
  %6 = call i1 @_f__void__bool__functionFalse()
  br label %lor.exit.L16

lor.exit.L16:                                     ; preds = %lor.1.L16, %land.exit.L13
  %lor_phi = phi i1 [ %5, %land.exit.L13 ], [ %6, %lor.1.L16 ]
  %7 = zext i1 %lor_phi to i32
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

attributes #0 = { noinline nounwind optnone uwtable }
