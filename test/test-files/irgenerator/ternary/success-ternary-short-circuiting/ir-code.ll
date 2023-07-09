; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i1 @_Z10condition1v() {
  %result = alloca i1, align 1
  ret i1 false
}

define private i1 @_Z10condition2v() {
  %result = alloca i1, align 1
  ret i1 true
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z10condition1v()
  br i1 %1, label %land.1.L10C26, label %land.exit.L10C26

land.1.L10C26:                                    ; preds = %0
  %2 = call i1 @_Z10condition2v()
  br label %land.exit.L10C26

land.exit.L10C26:                                 ; preds = %land.1.L10C26, %0
  %land_phi = phi i1 [ %1, %0 ], [ %2, %land.1.L10C26 ]
  %3 = select i1 %land_phi, i32 2, i32 3
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
