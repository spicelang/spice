; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.1 = private unnamed_addr constant [6 x i8] c"World\00", align 1
@anon.array.0 = private unnamed_addr constant [2 x ptr] [ptr @anon.string.0, ptr @anon.string.1]
@anon.string.2 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition '*iPtr == 13' evaluated to false.\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca [2 x ptr], align 8
  %test = alloca i32, align 4
  %i = alloca i32, align 4
  %iPtr = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  call void @_p__void__void__printFormat__double(double 1.123000e+00)
  call void @_p__void__void__printFormat__int(i32 543)
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.array.0, i64 16, i1 false)
  call void @_p__void__void__printFormat__stringarray([2 x ptr] [ptr @anon.string.0, ptr @anon.string.1])
  store i32 1234, ptr %test, align 4
  call void @_p__void__void__printFormat__intptr(ptr %test)
  store i32 12, ptr %i, align 4
  %2 = call ptr @_f__void__intptr__getAInc__intptr(ptr %i)
  store ptr %2, ptr %iPtr, align 8
  %3 = load ptr, ptr %iPtr, align 8
  %4 = load i32, ptr %3, align 4
  %5 = icmp eq i32 %4, 13
  br i1 %5, label %assert.exit.L12, label %assert.then.L12

assert.then.L12:                                  ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  %7 = load i32, ptr %result, align 4
  ret i32 %7
}

declare void @_p__void__void__printFormat__double(double)

declare void @_p__void__void__printFormat__int(i32)

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare void @_p__void__void__printFormat__stringarray([2 x ptr])

declare void @_p__void__void__printFormat__intptr(ptr)

declare ptr @_f__void__intptr__getAInc__intptr(ptr)

declare i32 @printf(ptr noundef, ...)

declare void @exit(i32)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
