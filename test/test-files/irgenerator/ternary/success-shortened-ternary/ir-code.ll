; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [12 x i8] c"F1 called.\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"F2 called.\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [11 x i8] c"Result: %d\00", align 1

define private i1 @_Z2f1v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i1 false
}

declare i32 @printf(ptr noundef, ...)

define private i1 @_Z2f2v() {
  %result = alloca i1, align 1
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  ret i1 true
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i1 @_Z2f1v()
  %2 = call i1 @_Z2f2v()
  %3 = select i1 %1, i1 %1, i1 %2
  %4 = zext i1 %3 to i32
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

attributes #0 = { noinline nounwind optnone uwtable }
