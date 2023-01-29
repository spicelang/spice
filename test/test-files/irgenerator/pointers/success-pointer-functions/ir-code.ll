; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Person__string_string_int = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@anon.struct.0 = private unnamed_addr constant %__Person__string_string_int { ptr @anon.string.0, ptr @anon.string.1, i32 32 }
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

define private void @_p__void__void__birthday__Personptr(ptr %0) {
  %person = alloca ptr, align 8
  store ptr %0, ptr %person, align 8
  %2 = load ptr, ptr %person, align 8
  %age = getelementptr inbounds %__Person__string_string_int, ptr %2, i32 0, i32 2
  %3 = load i32, ptr %age, align 4
  %4 = add i32 %3, 1
  store i32 %4, ptr %age, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__Person__string_string_int, align 8
  %mike = alloca %__Person__string_string_int, align 8
  store i32 0, ptr %result, align 4
  call void @llvm.memcpy.p0.p0.i64(ptr %1, ptr @anon.struct.0, i64 24, i1 false)
  store %__Person__string_string_int { ptr @anon.string.0, ptr @anon.string.1, i32 32 }, ptr %mike, align 8
  %lastName = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 1
  %2 = load ptr, ptr %lastName, align 8
  %firstName = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 0
  %3 = load ptr, ptr %firstName, align 8
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %2, ptr %3)
  %age = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 2
  %5 = load i32, ptr %age, align 4
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %5)
  call void @_p__void__void__birthday__Personptr(ptr %mike)
  %age1 = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 2
  %7 = load i32, ptr %age1, align 4
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %7)
  %9 = load i32, ptr %result, align 4
  ret i32 %9
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #1

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
