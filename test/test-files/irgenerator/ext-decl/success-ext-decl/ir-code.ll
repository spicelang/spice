; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@MODE_ALL_RWX = private constant i32 511

declare ptr @malloc(i64)

declare void @free(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %address = alloca ptr, align 8
  store i32 0, ptr %result, align 4
  %1 = call ptr @malloc(i64 1)
  store ptr %1, ptr %address, align 8
  %2 = load ptr, ptr %address, align 8
  store i8 12, ptr %address, align 1
  %3 = load ptr, ptr %address, align 8
  call void @free(ptr %3)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

attributes #0 = { noinline nounwind optnone uwtable }
