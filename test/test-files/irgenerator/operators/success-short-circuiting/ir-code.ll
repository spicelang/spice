; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"Function True\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Function False\0A\00", align 1
@2 = private unnamed_addr constant [30 x i8] c"Logical and evaluated to: %d\0A\00", align 1
@3 = private unnamed_addr constant [29 x i8] c"Logical or evaluated to: %d\0A\00", align 1

define internal i1 @_f__void__bool__functionTrue() {
entry.l1:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @0)
  store i1 true, ptr %0, align 1
  %2 = load i1, ptr %0, align 1
  ret i1 %2
}

declare i32 @printf(ptr, ...)

define internal i1 @_f__void__bool__functionFalse() {
entry.l6:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr @1)
  store i1 false, ptr %0, align 1
  %2 = load i1, ptr %0, align 1
  ret i1 %2
}

define i32 @main() {
entry.l11:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i1, align 1
  store i32 0, ptr %result, align 4
  %6 = call i1 @_f__void__bool__functionFalse()
  store i1 %6, ptr %0, align 1
  %7 = load i1, ptr %0, align 1
  br i1 %7, label %land.1.l13, label %land.end.l13

land.1.l13:                                       ; preds = %entry.l11
  %8 = call i1 @_f__void__bool__functionTrue()
  store i1 %8, ptr %1, align 1
  %9 = load i1, ptr %1, align 1
  br label %land.end.l13

land.end.l13:                                     ; preds = %land.1.l13, %entry.l11
  %land_phi = phi i1 [ %7, %entry.l11 ], [ %9, %land.1.l13 ]
  store i1 %land_phi, ptr %2, align 1
  %10 = load i1, ptr %2, align 1
  %11 = zext i1 %10 to i32
  %12 = call i32 (ptr, ...) @printf(ptr @2, i32 %11)
  %13 = call i1 @_f__void__bool__functionTrue()
  store i1 %13, ptr %3, align 1
  %14 = load i1, ptr %3, align 1
  br i1 %14, label %lor.end.l16, label %lor.1.l16

lor.1.l16:                                        ; preds = %land.end.l13
  %15 = call i1 @_f__void__bool__functionFalse()
  store i1 %15, ptr %4, align 1
  %16 = load i1, ptr %4, align 1
  br label %lor.end.l16

lor.end.l16:                                      ; preds = %lor.1.l16, %land.end.l13
  %lor_phi = phi i1 [ %14, %land.end.l13 ], [ %16, %lor.1.l16 ]
  store i1 %lor_phi, ptr %5, align 1
  %17 = load i1, ptr %5, align 1
  %18 = zext i1 %17 to i32
  %19 = call i32 (ptr, ...) @printf(ptr @3, i32 %18)
  %20 = load i32, ptr %result, align 4
  ret i32 %20
}
