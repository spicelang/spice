; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__Person__string_string_int = type { ptr, ptr, i32 }

@anon.string.0 = private unnamed_addr constant [5 x i8] c"Mike\00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"Miller\00", align 1
@printf.str.0 = private unnamed_addr constant [16 x i8] c"Person: %s, %s\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [25 x i8] c"Age before birthday: %d\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [24 x i8] c"Age after birthday: %d\0A\00", align 1

define private void @_p__void__void__birthday__Personptr(ptr %0) {
  %person = alloca ptr, align 8
  store ptr %0, ptr %person, align 8
  %2 = load ptr, ptr %person, align 8
  %age_addr = getelementptr inbounds %__Person__string_string_int, ptr %2, i32 0, i32 2
  %3 = load i32, ptr %age_addr, align 4
  %4 = add i32 %3, 1
  store i32 %4, ptr %age_addr, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %mike = alloca %__Person__string_string_int, align 8
  store i32 0, ptr %result, align 4
  %1 = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 0
  store ptr @anon.string.0, ptr %1, align 8
  %2 = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 1
  store ptr @anon.string.1, ptr %2, align 8
  %3 = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 2
  store i32 32, ptr %3, align 4
  %lastName_addr = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 1
  %4 = load ptr, ptr %lastName_addr, align 8
  %firstName_addr = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 0
  %5 = load ptr, ptr %firstName_addr, align 8
  %6 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr %4, ptr %5)
  %age_addr = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 2
  %7 = load i32, ptr %age_addr, align 4
  %8 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %7)
  call void @_p__void__void__birthday__Personptr(ptr %mike)
  %age_addr1 = getelementptr inbounds %__Person__string_string_int, ptr %mike, i32 0, i32 2
  %9 = load i32, ptr %age_addr1, align 4
  %10 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 %9)
  %11 = load i32, ptr %result, align 4
  ret i32 %11
}

declare i32 @printf(ptr noundef, ...)

attributes #0 = { noinline nounwind optnone uwtable }
