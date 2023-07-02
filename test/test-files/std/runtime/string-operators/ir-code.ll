; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%__String__charptr_long_long = type { ptr, i64, i64 }

@printf.str.0 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.0 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.1 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@anon.string.2 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@anon.string.3 = private unnamed_addr constant [7 x i8] c"World!\00", align 1
@printf.str.1 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.2 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.4 = private unnamed_addr constant [5 x i8] c" Hi!\00", align 1
@printf.str.3 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.5 = private unnamed_addr constant [5 x i8] c"Hi! \00", align 1
@printf.str.4 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.5 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.6 = private unnamed_addr constant [2 x i8] c" \00", align 1
@printf.str.6 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.7 = private unnamed_addr constant [8 x i8] c"Prefix \00", align 1
@anon.string.8 = private unnamed_addr constant [8 x i8] c" Suffix\00", align 1
@printf.str.7 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.9 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.10 = private unnamed_addr constant [7 x i8] c"Hello \00", align 1
@printf.str.8 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.9 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.10 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@printf.str.11 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.11 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.12 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.12 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.13 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.14 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.13 = private unnamed_addr constant [15 x i8] c"Equal raw: %d\0A\00", align 1
@anon.string.15 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.16 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.14 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.17 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.18 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.15 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.19 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.20 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.16 = private unnamed_addr constant [11 x i8] c"Equal: %d\0A\00", align 1
@anon.string.21 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.22 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.17 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.23 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.24 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.18 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.25 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.26 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.19 = private unnamed_addr constant [19 x i8] c"Non-equal raw: %d\0A\00", align 1
@anon.string.27 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.28 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.20 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.29 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1
@anon.string.30 = private unnamed_addr constant [19 x i8] c"Hello Programmers!\00", align 1
@printf.str.21 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.31 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.32 = private unnamed_addr constant [6 x i8] c"Hell2\00", align 1
@printf.str.22 = private unnamed_addr constant [15 x i8] c"Non-equal: %d\0A\00", align 1
@anon.string.33 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.34 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@anon.string.35 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@printf.str.23 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.36 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@anon.string.37 = private unnamed_addr constant [8 x i8] c" World!\00", align 1
@printf.str.24 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1
@anon.string.38 = private unnamed_addr constant [3 x i8] c"Hi\00", align 1
@printf.str.25 = private unnamed_addr constant [12 x i8] c"Result: %s\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %result = alloca i32, align 4
  %1 = alloca %__String__charptr_long_long, align 8
  %2 = alloca %__String__charptr_long_long, align 8
  %3 = alloca %__String__charptr_long_long, align 8
  %4 = alloca %__String__charptr_long_long, align 8
  %5 = alloca %__String__charptr_long_long, align 8
  %s1 = alloca %__String__charptr_long_long, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %__String__charptr_long_long, align 8
  %8 = alloca %__String__charptr_long_long, align 8
  %9 = alloca %__String__charptr_long_long, align 8
  %10 = alloca %__String__charptr_long_long, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %__String__charptr_long_long, align 8
  %13 = alloca %__String__charptr_long_long, align 8
  %14 = alloca %__String__charptr_long_long, align 8
  %15 = alloca %__String__charptr_long_long, align 8
  %16 = alloca ptr, align 8
  %17 = alloca %__String__charptr_long_long, align 8
  %18 = alloca %__String__charptr_long_long, align 8
  %19 = alloca %__String__charptr_long_long, align 8
  %20 = alloca %__String__charptr_long_long, align 8
  %s2 = alloca %__String__charptr_long_long, align 8
  %21 = alloca %__String__charptr_long_long, align 8
  %22 = alloca %__String__charptr_long_long, align 8
  %23 = alloca %__String__charptr_long_long, align 8
  %24 = alloca %__String__charptr_long_long, align 8
  %s3 = alloca %__String__charptr_long_long, align 8
  %25 = alloca %__String__charptr_long_long, align 8
  %26 = alloca %__String__charptr_long_long, align 8
  %27 = alloca %__String__charptr_long_long, align 8
  %28 = alloca %__String__charptr_long_long, align 8
  %29 = alloca %__String__charptr_long_long, align 8
  %30 = alloca %__String__charptr_long_long, align 8
  %31 = alloca %__String__charptr_long_long, align 8
  %32 = alloca %__String__charptr_long_long, align 8
  %33 = alloca %__String__charptr_long_long, align 8
  %34 = alloca %__String__charptr_long_long, align 8
  %35 = alloca %__String__charptr_long_long, align 8
  %36 = alloca %__String__charptr_long_long, align 8
  %s4 = alloca %__String__charptr_long_long, align 8
  %37 = alloca i8, align 1
  %s5 = alloca %__String__charptr_long_long, align 8
  %38 = alloca ptr, align 8
  %s6 = alloca %__String__charptr_long_long, align 8
  store i32 0, ptr %result, align 4
  call void @_mp__String__void__ctor__string(ptr %1, ptr @anon.string.0)
  call void @_mp__String__void__ctor__string(ptr %2, ptr @anon.string.1)
  %39 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %1, ptr %2)
  store %__String__charptr_long_long %39, ptr %3, align 8
  %40 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, %__String__charptr_long_long %39)
  call void @_mp__String__void__ctor__string(ptr %4, ptr @anon.string.2)
  call void @_mp__String__void__ctor__string(ptr %5, ptr @anon.string.3)
  %41 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %4, ptr %5)
  store %__String__charptr_long_long %41, ptr %s1, align 8
  store %__String__charptr_long_long %41, ptr %s1, align 8
  %42 = load %__String__charptr_long_long, ptr %s1, align 8
  %43 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, %__String__charptr_long_long %42)
  store ptr @anon.string.4, ptr %6, align 8
  %44 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %s1, ptr %6)
  store %__String__charptr_long_long %44, ptr %7, align 8
  %45 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, %__String__charptr_long_long %44)
  call void @_mp__String__void__ctor__string(ptr %8, ptr @anon.string.5)
  %46 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %8, ptr %s1)
  store %__String__charptr_long_long %46, ptr %9, align 8
  %47 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, %__String__charptr_long_long %46)
  %48 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %s1, ptr %s1)
  store %__String__charptr_long_long %48, ptr %10, align 8
  %49 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, %__String__charptr_long_long %48)
  store ptr @anon.string.6, ptr %11, align 8
  %50 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %s1, ptr %11)
  store %__String__charptr_long_long %50, ptr %12, align 8
  %51 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %12, ptr %s1)
  store %__String__charptr_long_long %51, ptr %13, align 8
  %52 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, %__String__charptr_long_long %51)
  call void @_mp__String__void__ctor__string(ptr %14, ptr @anon.string.7)
  %53 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr %14, ptr %s1)
  store %__String__charptr_long_long %53, ptr %15, align 8
  store ptr @anon.string.8, ptr %16, align 8
  %54 = call %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr %15, ptr %16)
  store %__String__charptr_long_long %54, ptr %17, align 8
  %55 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, %__String__charptr_long_long %54)
  call void @_mp__String__void__ctor__string(ptr %18, ptr @anon.string.9)
  %56 = call %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16 4, ptr %18)
  store %__String__charptr_long_long %56, ptr %19, align 8
  %57 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, %__String__charptr_long_long %56)
  call void @_mp__String__void__ctor__string(ptr %20, ptr @anon.string.10)
  %58 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr %20, i32 5)
  store %__String__charptr_long_long %58, ptr %s2, align 8
  store %__String__charptr_long_long %58, ptr %s2, align 8
  %59 = load %__String__charptr_long_long, ptr %s2, align 8
  %60 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, %__String__charptr_long_long %59)
  call void @_mp__String__void__ctor__char(ptr %21, i8 97)
  %61 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 20, ptr %21)
  store %__String__charptr_long_long %61, ptr %22, align 8
  %62 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, %__String__charptr_long_long %61)
  call void @_mp__String__void__ctor__char(ptr %23, i8 99)
  %63 = call %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32 2, ptr %23)
  store %__String__charptr_long_long %63, ptr %24, align 8
  %64 = call %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr %24, i32 7)
  store %__String__charptr_long_long %64, ptr %s3, align 8
  store %__String__charptr_long_long %64, ptr %s3, align 8
  %65 = load %__String__charptr_long_long, ptr %s3, align 8
  %66 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, %__String__charptr_long_long %65)
  %67 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.11, ptr @anon.string.12)
  %68 = zext i1 %67 to i32
  %69 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 %68)
  %70 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.13, ptr @anon.string.14)
  %71 = zext i1 %70 to i32
  %72 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %71)
  %73 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.15, ptr @anon.string.16)
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %74)
  call void @_mp__String__void__ctor__string(ptr %25, ptr @anon.string.17)
  call void @_mp__String__void__ctor__string(ptr %26, ptr @anon.string.18)
  %76 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %25, ptr %26)
  %77 = zext i1 %76 to i32
  %78 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %77)
  call void @_mp__String__void__ctor__string(ptr %27, ptr @anon.string.19)
  call void @_mp__String__void__ctor__string(ptr %28, ptr @anon.string.20)
  %79 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %27, ptr %28)
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %80)
  call void @_mp__String__void__ctor__string(ptr %29, ptr @anon.string.21)
  call void @_mp__String__void__ctor__string(ptr %30, ptr @anon.string.22)
  %82 = call i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr %29, ptr %30)
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %83)
  %85 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.23, ptr @anon.string.24)
  %86 = xor i1 %85, true
  %87 = zext i1 %86 to i32
  %88 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %87)
  %89 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.25, ptr @anon.string.26)
  %90 = xor i1 %89, true
  %91 = zext i1 %90 to i32
  %92 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %91)
  %93 = call i1 @_f__void__bool__isRawEqual__string_string(ptr @anon.string.27, ptr @anon.string.28)
  %94 = xor i1 %93, true
  %95 = zext i1 %94 to i32
  %96 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %95)
  call void @_mp__String__void__ctor__string(ptr %31, ptr @anon.string.29)
  call void @_mp__String__void__ctor__string(ptr %32, ptr @anon.string.30)
  %97 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %31, ptr %32)
  %98 = zext i1 %97 to i32
  %99 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %98)
  call void @_mp__String__void__ctor__string(ptr %33, ptr @anon.string.31)
  call void @_mp__String__void__ctor__string(ptr %34, ptr @anon.string.32)
  %100 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %33, ptr %34)
  %101 = zext i1 %100 to i32
  %102 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %101)
  call void @_mp__String__void__ctor__string(ptr %35, ptr @anon.string.33)
  call void @_mp__String__void__ctor__string(ptr %36, ptr @anon.string.34)
  %103 = call i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr %35, ptr %36)
  %104 = zext i1 %103 to i32
  %105 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %104)
  call void @_mp__String__void__ctor__string(ptr %s4, ptr @anon.string.35)
  store i8 108, ptr %37, align 1
  call void @_p__void__void__op.plusequal__Stringref_charref(ptr %s4, ptr %37)
  %106 = load %__String__charptr_long_long, ptr %s4, align 8
  %107 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.23, %__String__charptr_long_long %106)
  call void @_mp__String__void__ctor__string(ptr %s5, ptr @anon.string.36)
  store ptr @anon.string.37, ptr %38, align 8
  call void @_p__void__void__op.plusequal__Stringref_stringref(ptr %s5, ptr %38)
  %108 = load %__String__charptr_long_long, ptr %s5, align 8
  %109 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.24, %__String__charptr_long_long %108)
  call void @_mp__String__void__ctor__string(ptr %s6, ptr @anon.string.38)
  call void @_p__void__void__op.mulequal__Stringref_int(ptr %s6, i32 3)
  %110 = load %__String__charptr_long_long, ptr %s6, align 8
  %111 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.25, %__String__charptr_long_long %110)
  call void @_mp__String__void__dtor(ptr %36)
  call void @_mp__String__void__dtor(ptr %33)
  call void @_mp__String__void__dtor(ptr %31)
  call void @_mp__String__void__dtor(ptr %32)
  call void @_mp__String__void__dtor(ptr %10)
  call void @_mp__String__void__dtor(ptr %15)
  call void @_mp__String__void__dtor(ptr %24)
  call void @_mp__String__void__dtor(ptr %26)
  call void @_mp__String__void__dtor(ptr %12)
  call void @_mp__String__void__dtor(ptr %7)
  call void @_mp__String__void__dtor(ptr %28)
  call void @_mp__String__void__dtor(ptr %5)
  call void @_mp__String__void__dtor(ptr %35)
  call void @_mp__String__void__dtor(ptr %2)
  call void @_mp__String__void__dtor(ptr %s6)
  call void @_mp__String__void__dtor(ptr %s4)
  call void @_mp__String__void__dtor(ptr %17)
  call void @_mp__String__void__dtor(ptr %s5)
  call void @_mp__String__void__dtor(ptr %3)
  call void @_mp__String__void__dtor(ptr %34)
  call void @_mp__String__void__dtor(ptr %s3)
  call void @_mp__String__void__dtor(ptr %s1)
  call void @_mp__String__void__dtor(ptr %13)
  call void @_mp__String__void__dtor(ptr %s2)
  call void @_mp__String__void__dtor(ptr %18)
  call void @_mp__String__void__dtor(ptr %19)
  call void @_mp__String__void__dtor(ptr %23)
  call void @_mp__String__void__dtor(ptr %21)
  call void @_mp__String__void__dtor(ptr %22)
  call void @_mp__String__void__dtor(ptr %25)
  call void @_mp__String__void__dtor(ptr %27)
  call void @_mp__String__void__dtor(ptr %9)
  call void @_mp__String__void__dtor(ptr %30)
  call void @_mp__String__void__dtor(ptr %29)
  %112 = load i32, ptr %result, align 4
  ret i32 %112
}

declare i32 @printf(ptr noundef, ...)

declare void @_mp__String__void__ctor__string(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_Stringref(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.plus__Stringref_stringref(ptr, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__short_Stringref(i16, ptr)

declare %__String__charptr_long_long @_f__void__String__op.mul__Stringref_int(ptr, i32)

declare void @_mp__String__void__ctor__char(ptr, i8)

declare %__String__charptr_long_long @_f__void__String__op.mul__int_Stringref(i32, ptr)

declare i1 @_f__void__bool__isRawEqual__string_string(ptr, ptr)

declare i1 @_f__void__bool__op.equal__Stringref_Stringref(ptr, ptr)

declare i1 @_f__void__bool__op.notequal__Stringref_Stringref(ptr, ptr)

declare void @_p__void__void__op.plusequal__Stringref_charref(ptr, ptr)

declare void @_p__void__void__op.plusequal__Stringref_stringref(ptr, ptr)

declare void @_p__void__void__op.mulequal__Stringref_int(ptr, i32)

declare void @_mp__String__void__dtor(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
