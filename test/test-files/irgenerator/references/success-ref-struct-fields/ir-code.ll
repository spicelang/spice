; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.TestStruct = type { i1, ptr }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Before: %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [10 x i8] c"After: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %t = alloca i32, align 4
  %ts = alloca %struct.TestStruct, align 8
  store i32 0, ptr %result, align 4
  store i32 123, ptr %t, align 4
  %1 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 0
  store i1 true, ptr %1, align 1
  %2 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  store ptr %t, ptr %2, align 8
  %f2_addr = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  %3 = load ptr, ptr %f2_addr, align 8
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4)
  %f2_addr1 = getelementptr inbounds %struct.TestStruct, ptr %ts, i32 0, i32 1
  %6 = load ptr, ptr %f2_addr1, align 8
  %7 = load i32, ptr %6, align 4
  %8 = add i32 %7, 1
  store i32 %8, ptr %6, align 4
  %9 = load i32, ptr %t, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %9)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
