; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"Argc: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [16 x i8] c"Argv no. 0: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [16 x i8] c"Argv no. 1: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  store i32 %0, ptr %argc, align 4
  store ptr %1, ptr %argv, align 8
  %3 = load i32, ptr %argc, align 4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %3)
  %5 = load ptr, ptr %argv, align 8
  %6 = getelementptr inbounds ptr, ptr %5, i32 0
  %7 = load ptr, ptr %6, align 8
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, ptr %7)
  %9 = load i32, ptr %argc, align 4
  %10 = icmp sgt i32 %9, 1
  br i1 %10, label %if.then.L4, label %if.exit.L4

if.then.L4:                                       ; preds = %2
  %11 = load ptr, ptr %argv, align 8
  %12 = getelementptr inbounds ptr, ptr %11, i32 1
  %13 = load ptr, ptr %12, align 8
  %14 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, ptr %13)
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %if.then.L4, %2
  %15 = load i32, ptr %result, align 4
  ret i32 %15
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
