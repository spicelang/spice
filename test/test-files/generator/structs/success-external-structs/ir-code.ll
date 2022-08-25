; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%_s__Vec__int_bool = type { i32, i1 }

define i32 @main() {
entry.l3:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %0 = alloca %_s__Vec__int_bool, align 8
  %1 = getelementptr inbounds %_s__Vec__int_bool, ptr %0, i32 0, i32 0
  store i32 11, ptr %1, align 4
  %2 = getelementptr inbounds %_s__Vec__int_bool, ptr %0, i32 0, i32 1
  store i1 false, ptr %2, align 1
  call void @_mp__Vec__print(ptr %0)
  %3 = alloca i1, align 1
  store i1 true, ptr %3, align 1
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

declare void @_mp__Vec__print(ptr)
