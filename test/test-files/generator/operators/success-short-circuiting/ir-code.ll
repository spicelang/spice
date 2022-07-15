; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [15 x i8] c"Function True\0A\00", align 1
@1 = private unnamed_addr constant [16 x i8] c"Function False\0A\00", align 1
@2 = private unnamed_addr constant [30 x i8] c"Logical and evaluated to: %d\0A\00", align 1
@3 = private unnamed_addr constant [29 x i8] c"Logical or evaluated to: %d\0A\00", align 1

define internal i1 @_f__void__functionTrue() {
entry:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @0, i32 0, i32 0))
  store i1 true, i1* %0, align 1
  %2 = load i1, i1* %0, align 1
  ret i1 %2
}

declare i32 @printf(i8*, ...)

define internal i1 @_f__void__functionFalse() {
entry:
  %result = alloca i1, align 1
  %0 = alloca i1, align 1
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @1, i32 0, i32 0))
  store i1 false, i1* %0, align 1
  %2 = load i1, i1* %0, align 1
  ret i1 %2
}

define i32 @main() {
entry:
  %result = alloca i32, align 4
  %0 = alloca i1, align 1
  %1 = alloca i1, align 1
  %2 = alloca i1, align 1
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  %6 = call i1 @_f__void__functionFalse()
  store i1 %6, i1* %0, align 1
  %7 = load i1, i1* %0, align 1
  br i1 %7, label %land.1, label %land.end

land.1:                                           ; preds = %entry
  %8 = call i1 @_f__void__functionTrue()
  store i1 %8, i1* %1, align 1
  %9 = load i1, i1* %1, align 1
  br label %land.end

land.end:                                         ; preds = %land.1, %entry
  %land_phi = phi i1 [ %7, %entry ], [ %9, %land.1 ]
  store i1 %land_phi, i1* %2, align 1
  %10 = load i1, i1* %2, align 1
  %11 = zext i1 %10 to i32
  %12 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @2, i32 0, i32 0), i32 %11)
  %13 = call i1 @_f__void__functionTrue()
  store i1 %13, i1* %3, align 1
  %14 = load i1, i1* %3, align 1
  br i1 %14, label %lor.end, label %lor.1

lor.1:                                            ; preds = %land.end
  %15 = call i1 @_f__void__functionFalse()
  store i1 %15, i1* %4, align 1
  %16 = load i1, i1* %4, align 1
  br label %lor.end

lor.end:                                          ; preds = %lor.1, %land.end
  %lor_phi = phi i1 [ %14, %land.end ], [ %16, %lor.1 ]
  store i1 %lor_phi, i1* %5, align 1
  %17 = load i1, i1* %5, align 1
  %18 = zext i1 %17 to i32
  %19 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([29 x i8], [29 x i8]* @3, i32 0, i32 0), i32 %18)
  %20 = load i32, i32* %result, align 4
  ret i32 %20
}
