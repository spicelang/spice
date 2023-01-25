; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [22 x i8] c"Starting threads ...\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [24 x i8] c"Starting thread %d ...\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [23 x i8] c"Hello from the thread\0A\00", align 1
@printf.str.3 = private unnamed_addr constant [21 x i8] c"Hello from original\0A\00", align 1

declare i32 @usleep(i32)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca { ptr }, align 8
  store i32 0, ptr %result, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  store i32 1, ptr %i, align 4
  br label %for.head.L5

for.head.L5:                                      ; preds = %for.tail.L5, %0
  %3 = load i32, ptr %i, align 4
  %4 = icmp sle i32 %3, 8
  br i1 %4, label %for.body.L5, label %for.exit.L5

for.body.L5:                                      ; preds = %for.head.L5
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  %7 = getelementptr inbounds { ptr }, ptr %1, i32 0, i32 0
  store ptr %i, ptr %7, align 8
  %8 = alloca ptr, align 8
  %9 = call i32 @pthread_create(ptr %8, ptr null, ptr @_thread0, ptr %1)
  br label %for.tail.L5

for.tail.L5:                                      ; preds = %for.body.L5
  %10 = load i32, ptr %i, align 4
  %11 = add i32 %10, 1
  store i32 %11, ptr %i, align 4
  br label %for.head.L5

for.exit.L5:                                      ; preds = %for.head.L5
  %12 = call i32 @usleep(i32 1000000)
  %13 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  %14 = load i32, ptr %result, align 4
  ret i32 %14
}

declare i32 @printf(ptr noundef, ...)

define private ptr @_thread0(ptr %0) {
  %2 = getelementptr inbounds { ptr }, ptr %0, i32 0, i32 0
  %3 = load ptr, ptr %2, align 8
  %4 = load i32, ptr %3, align 4
  %5 = mul i32 100, %4
  %6 = mul i32 %5, 1000
  %7 = call i32 @usleep(i32 %6)
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

attributes #0 = { noinline nounwind optnone uwtable }
