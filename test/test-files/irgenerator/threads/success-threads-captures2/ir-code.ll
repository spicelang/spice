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
  %capturedVariable = alloca i32, align 4
  %i = alloca i32, align 4
  %1 = alloca { ptr, ptr }, align 8
  store i32 0, ptr %result, align 4
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  store i32 0, ptr %capturedVariable, align 4
  store i32 1, ptr %i, align 4
  br label %for.head.L6

for.head.L6:                                      ; preds = %for.tail.L6, %0
  %3 = load i32, ptr %i, align 4
  %4 = icmp sle i32 %3, 8
  br i1 %4, label %for.body.L6, label %for.exit.L6

for.body.L6:                                      ; preds = %for.head.L6
  %5 = load i32, ptr %i, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  %7 = getelementptr inbounds { ptr, ptr }, ptr %1, i32 0, i32 0
  store ptr %capturedVariable, ptr %7, align 8
  %8 = getelementptr inbounds { ptr, ptr }, ptr %1, i32 0, i32 1
  store ptr %i, ptr %8, align 8
  %9 = alloca ptr, align 8
  %10 = call i32 @pthread_create(ptr %9, ptr null, ptr @_thread0, ptr %1)
  br label %for.tail.L6

for.tail.L6:                                      ; preds = %for.body.L6
  %11 = load i32, ptr %i, align 4
  %12 = add i32 %11, 1
  store i32 %12, ptr %i, align 4
  br label %for.head.L6

for.exit.L6:                                      ; preds = %for.head.L6
  %13 = call i32 @usleep(i32 1000000)
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

declare i32 @printf(ptr noundef, ...)

define private ptr @_thread0(ptr %0) {
thread.entry.L8:
  %1 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 0
  %2 = load ptr, ptr %1, align 8
  %3 = getelementptr inbounds { ptr, ptr }, ptr %0, i32 0, i32 1
  %4 = load ptr, ptr %3, align 8
  %5 = load i32, ptr %4, align 4
  %6 = mul i32 100, %5
  %7 = mul i32 %6, 1000
  %8 = call i32 @usleep(i32 %7)
  %9 = load i32, ptr %2, align 4
  %10 = mul i32 %9, 2
  store volatile i32 %10, ptr %2, align 4
  %11 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  ret ptr null
}

declare i32 @pthread_create(ptr, ptr, ptr, ptr)

attributes #0 = { noinline nounwind optnone uwtable }
