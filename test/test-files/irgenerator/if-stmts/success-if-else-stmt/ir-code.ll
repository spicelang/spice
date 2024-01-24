; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [10 x i8] c"If branch\00", align 1
@printf.str.1 = private unnamed_addr constant [10 x i8] c"Else if 1\00", align 1
@printf.str.2 = private unnamed_addr constant [10 x i8] c"Else if 2\00", align 1
@printf.str.3 = private unnamed_addr constant [5 x i8] c"Else\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  br i1 false, label %if.then.L2, label %if.else.L2

if.then.L2:                                       ; preds = %0
  %1 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  br label %if.exit.L2

if.else.L2:                                       ; preds = %0
  br i1 false, label %if.then.L4, label %if.else.L4

if.then.L4:                                       ; preds = %if.else.L2
  %2 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1)
  br label %if.exit.L4

if.else.L4:                                       ; preds = %if.else.L2
  br i1 false, label %if.then.L6, label %if.else.L6

if.then.L6:                                       ; preds = %if.else.L4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2)
  br label %if.exit.L6

if.else.L6:                                       ; preds = %if.else.L4
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3)
  br label %if.exit.L6

if.exit.L6:                                       ; preds = %if.else.L6, %if.then.L6
  br label %if.exit.L4

if.exit.L4:                                       ; preds = %if.exit.L6, %if.then.L4
  br label %if.exit.L2

if.exit.L2:                                       ; preds = %if.exit.L4, %if.then.L2
  %5 = load i32, ptr %result, align 4
  ret i32 %5
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
