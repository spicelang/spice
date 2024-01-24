; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Test func 1\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [17 x i8] c"Test func 2: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [12 x i8] c"Result: %d\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [12 x i8] c"param value\00", align 1

define private i32 @_Z8testFuncv() {
  %result = alloca i32, align 4
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  ret i32 1
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #0

define private i32 @_Z8testFuncPc(ptr %0) {
  %result = alloca i32, align 4
  %param = alloca ptr, align 8
  store ptr %0, ptr %param, align 8
  %2 = load ptr, ptr %param, align 8
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %2)
  ret i32 2
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %res = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %1 = call i32 @_Z8testFuncv()
  store i32 %1, ptr %res, align 4
  %2 = load i32, ptr %res, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %2)
  %4 = call i32 @_Z8testFuncPc(ptr @anon.string.0)
  store i32 %4, ptr %res, align 4
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
