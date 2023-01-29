; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Person__string_string_int = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"John\00", align 1
@anon.string.1 = private unnamed_addr constant [4 x i8] c"Doe\00", align 1
@anon.struct.0 = private unnamed_addr constant %__Person__string_string_int { ptr @anon.string.0, ptr @anon.string.1, i32 46 }
@printf.str.0 = private unnamed_addr constant [15 x i8] c"John's age: %d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__Person__string_string_int, align 8
  %john = alloca %__Person__string_string_int, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.struct.0, i64 24, i1 false)
  store %__Person__string_string_int { ptr @anon.string.0, ptr @anon.string.1, i32 46 }, ptr %john, align 8
  %age = getelementptr inbounds %__Person__string_string_int, ptr %john, i32 0, i32 2
  store i32 47, ptr %age, align 4
  %age1 = getelementptr inbounds %__Person__string_string_int, ptr %john, i32 0, i32 2
  %2 = load i32, ptr %age1, align 4
  %3 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %2)
  %4 = load i32, ptr %result, align 4
  ret i32 %4
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
