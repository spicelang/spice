; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Struct = type { ptr }

@printf.str.0 = private unnamed_addr constant [16 x i8] c"Field value: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %str = alloca %struct.Struct, align 8
  %1 = alloca i32, align 4
  store i32 0, ptr %result, align 4
  store i32 123, ptr %1, align 4
  %2 = getelementptr inbounds %struct.Struct, ptr %str, i32 0, i32 0
  store ptr %1, ptr %2, align 8
  %ref_addr = getelementptr inbounds %struct.Struct, ptr %str, i32 0, i32 0
  %3 = load ptr, ptr %ref_addr, align 8
  %4 = load i32, ptr %3, align 4
  %5 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %4)
  %6 = load i32, ptr %result, align 4
  ret i32 %6
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
