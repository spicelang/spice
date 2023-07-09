; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.String = type { ptr, i64, i64 }

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
  %1 = alloca %struct.String, align 8
  %2 = alloca %struct.String, align 8
  %3 = alloca %struct.String, align 8
  %4 = alloca %struct.String, align 8
  %5 = alloca %struct.String, align 8
  %s1 = alloca %struct.String, align 8
  %6 = alloca ptr, align 8
  %7 = alloca %struct.String, align 8
  %8 = alloca %struct.String, align 8
  %9 = alloca %struct.String, align 8
  %10 = alloca %struct.String, align 8
  %11 = alloca ptr, align 8
  %12 = alloca %struct.String, align 8
  %13 = alloca %struct.String, align 8
  %14 = alloca %struct.String, align 8
  %15 = alloca %struct.String, align 8
  %16 = alloca ptr, align 8
  %17 = alloca %struct.String, align 8
  %18 = alloca %struct.String, align 8
  %19 = alloca %struct.String, align 8
  %20 = alloca %struct.String, align 8
  %s2 = alloca %struct.String, align 8
  %21 = alloca %struct.String, align 8
  %22 = alloca %struct.String, align 8
  %23 = alloca %struct.String, align 8
  %24 = alloca %struct.String, align 8
  %s3 = alloca %struct.String, align 8
  %25 = alloca %struct.String, align 8
  %26 = alloca %struct.String, align 8
  %27 = alloca %struct.String, align 8
  %28 = alloca %struct.String, align 8
  %29 = alloca %struct.String, align 8
  %30 = alloca %struct.String, align 8
  %31 = alloca %struct.String, align 8
  %32 = alloca %struct.String, align 8
  %33 = alloca %struct.String, align 8
  %34 = alloca %struct.String, align 8
  %35 = alloca %struct.String, align 8
  %36 = alloca %struct.String, align 8
  %s4 = alloca %struct.String, align 8
  %37 = alloca i8, align 1
  %s5 = alloca %struct.String, align 8
  %38 = alloca ptr, align 8
  %s6 = alloca %struct.String, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN6String4ctorEPc(ptr %1, ptr @anon.string.0)
  call void @_ZN6String4ctorEPc(ptr %2, ptr @anon.string.1)
  %39 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %1, ptr %2)
  store %struct.String %39, ptr %3, align 8
  %40 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, %struct.String %39)
  call void @_ZN6String4ctorEPc(ptr %4, ptr @anon.string.2)
  call void @_ZN6String4ctorEPc(ptr %5, ptr @anon.string.3)
  %41 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %4, ptr %5)
  store %struct.String %41, ptr %s1, align 8
  store %struct.String %41, ptr %s1, align 8
  %42 = load %struct.String, ptr %s1, align 8
  %43 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, %struct.String %42)
  store ptr @anon.string.4, ptr %6, align 8
  %44 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %s1, ptr %6)
  store %struct.String %44, ptr %7, align 8
  %45 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.2, %struct.String %44)
  call void @_ZN6String4ctorEPc(ptr %8, ptr @anon.string.5)
  %46 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %8, ptr %s1)
  store %struct.String %46, ptr %9, align 8
  %47 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.3, %struct.String %46)
  %48 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %s1, ptr %s1)
  store %struct.String %48, ptr %10, align 8
  %49 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.4, %struct.String %48)
  store ptr @anon.string.6, ptr %11, align 8
  %50 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %s1, ptr %11)
  store %struct.String %50, ptr %12, align 8
  %51 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %12, ptr %s1)
  store %struct.String %51, ptr %13, align 8
  %52 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.5, %struct.String %51)
  call void @_ZN6String4ctorEPc(ptr %14, ptr @anon.string.7)
  %53 = call %struct.String @_Z7op.plusRK6StringRK6String(ptr %14, ptr %s1)
  store %struct.String %53, ptr %15, align 8
  store ptr @anon.string.8, ptr %16, align 8
  %54 = call %struct.String @_Z7op.plusRK6StringRKPc(ptr %15, ptr %16)
  store %struct.String %54, ptr %17, align 8
  %55 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.6, %struct.String %54)
  call void @_ZN6String4ctorEPc(ptr %18, ptr @anon.string.9)
  %56 = call %struct.String @_Z6op.mulsRK6String(i16 4, ptr %18)
  store %struct.String %56, ptr %19, align 8
  %57 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.7, %struct.String %56)
  call void @_ZN6String4ctorEPc(ptr %20, ptr @anon.string.10)
  %58 = call %struct.String @_Z6op.mulRK6Stringi(ptr %20, i32 5)
  store %struct.String %58, ptr %s2, align 8
  store %struct.String %58, ptr %s2, align 8
  %59 = load %struct.String, ptr %s2, align 8
  %60 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.8, %struct.String %59)
  call void @_ZN6String4ctorEh(ptr %21, i8 97)
  %61 = call %struct.String @_Z6op.muliRK6String(i32 20, ptr %21)
  store %struct.String %61, ptr %22, align 8
  %62 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.9, %struct.String %61)
  call void @_ZN6String4ctorEh(ptr %23, i8 99)
  %63 = call %struct.String @_Z6op.muliRK6String(i32 2, ptr %23)
  store %struct.String %63, ptr %24, align 8
  %64 = call %struct.String @_Z6op.mulRK6Stringi(ptr %24, i32 7)
  store %struct.String %64, ptr %s3, align 8
  store %struct.String %64, ptr %s3, align 8
  %65 = load %struct.String, ptr %s3, align 8
  %66 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.10, %struct.String %65)
  %67 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.11, ptr @anon.string.12)
  %68 = zext i1 %67 to i32
  %69 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 %68)
  %70 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.13, ptr @anon.string.14)
  %71 = zext i1 %70 to i32
  %72 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i32 %71)
  %73 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.15, ptr @anon.string.16)
  %74 = zext i1 %73 to i32
  %75 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.13, i32 %74)
  call void @_ZN6String4ctorEPc(ptr %25, ptr @anon.string.17)
  call void @_ZN6String4ctorEPc(ptr %26, ptr @anon.string.18)
  %76 = call i1 @_Z8op.equalRK6StringRK6String(ptr %25, ptr %26)
  %77 = zext i1 %76 to i32
  %78 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.14, i32 %77)
  call void @_ZN6String4ctorEPc(ptr %27, ptr @anon.string.19)
  call void @_ZN6String4ctorEPc(ptr %28, ptr @anon.string.20)
  %79 = call i1 @_Z8op.equalRK6StringRK6String(ptr %27, ptr %28)
  %80 = zext i1 %79 to i32
  %81 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.15, i32 %80)
  call void @_ZN6String4ctorEPc(ptr %29, ptr @anon.string.21)
  call void @_ZN6String4ctorEPc(ptr %30, ptr @anon.string.22)
  %82 = call i1 @_Z8op.equalRK6StringRK6String(ptr %29, ptr %30)
  %83 = zext i1 %82 to i32
  %84 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.16, i32 %83)
  %85 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.23, ptr @anon.string.24)
  %86 = xor i1 %85, true
  %87 = zext i1 %86 to i32
  %88 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.17, i32 %87)
  %89 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.25, ptr @anon.string.26)
  %90 = xor i1 %89, true
  %91 = zext i1 %90 to i32
  %92 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.18, i32 %91)
  %93 = call i1 @_Z10isRawEqualPcPc(ptr @anon.string.27, ptr @anon.string.28)
  %94 = xor i1 %93, true
  %95 = zext i1 %94 to i32
  %96 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.19, i32 %95)
  call void @_ZN6String4ctorEPc(ptr %31, ptr @anon.string.29)
  call void @_ZN6String4ctorEPc(ptr %32, ptr @anon.string.30)
  %97 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %31, ptr %32)
  %98 = zext i1 %97 to i32
  %99 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.20, i32 %98)
  call void @_ZN6String4ctorEPc(ptr %33, ptr @anon.string.31)
  call void @_ZN6String4ctorEPc(ptr %34, ptr @anon.string.32)
  %100 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %33, ptr %34)
  %101 = zext i1 %100 to i32
  %102 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.21, i32 %101)
  call void @_ZN6String4ctorEPc(ptr %35, ptr @anon.string.33)
  call void @_ZN6String4ctorEPc(ptr %36, ptr @anon.string.34)
  %103 = call i1 @_Z11op.notequalRK6StringRK6String(ptr %35, ptr %36)
  %104 = zext i1 %103 to i32
  %105 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.22, i32 %104)
  call void @_ZN6String4ctorEPc(ptr %s4, ptr @anon.string.35)
  store i8 108, ptr %37, align 1
  call void @_Z12op.plusequalR6StringRKh(ptr %s4, ptr %37)
  %106 = load %struct.String, ptr %s4, align 8
  %107 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.23, %struct.String %106)
  call void @_ZN6String4ctorEPc(ptr %s5, ptr @anon.string.36)
  store ptr @anon.string.37, ptr %38, align 8
  call void @_Z12op.plusequalR6StringRKPc(ptr %s5, ptr %38)
  %108 = load %struct.String, ptr %s5, align 8
  %109 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.24, %struct.String %108)
  call void @_ZN6String4ctorEPc(ptr %s6, ptr @anon.string.38)
  call void @_Z11op.mulequalR6Stringi(ptr %s6, i32 3)
  %110 = load %struct.String, ptr %s6, align 8
  %111 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.25, %struct.String %110)
  call void @_ZN6String4dtorEv(ptr %36)
  call void @_ZN6String4dtorEv(ptr %33)
  call void @_ZN6String4dtorEv(ptr %31)
  call void @_ZN6String4dtorEv(ptr %32)
  call void @_ZN6String4dtorEv(ptr %10)
  call void @_ZN6String4dtorEv(ptr %15)
  call void @_ZN6String4dtorEv(ptr %24)
  call void @_ZN6String4dtorEv(ptr %26)
  call void @_ZN6String4dtorEv(ptr %12)
  call void @_ZN6String4dtorEv(ptr %7)
  call void @_ZN6String4dtorEv(ptr %28)
  call void @_ZN6String4dtorEv(ptr %5)
  call void @_ZN6String4dtorEv(ptr %35)
  call void @_ZN6String4dtorEv(ptr %2)
  call void @_ZN6String4dtorEv(ptr %s6)
  call void @_ZN6String4dtorEv(ptr %s4)
  call void @_ZN6String4dtorEv(ptr %17)
  call void @_ZN6String4dtorEv(ptr %s5)
  call void @_ZN6String4dtorEv(ptr %3)
  call void @_ZN6String4dtorEv(ptr %34)
  call void @_ZN6String4dtorEv(ptr %s3)
  call void @_ZN6String4dtorEv(ptr %s1)
  call void @_ZN6String4dtorEv(ptr %13)
  call void @_ZN6String4dtorEv(ptr %s2)
  call void @_ZN6String4dtorEv(ptr %18)
  call void @_ZN6String4dtorEv(ptr %19)
  call void @_ZN6String4dtorEv(ptr %23)
  call void @_ZN6String4dtorEv(ptr %21)
  call void @_ZN6String4dtorEv(ptr %22)
  call void @_ZN6String4dtorEv(ptr %25)
  call void @_ZN6String4dtorEv(ptr %27)
  call void @_ZN6String4dtorEv(ptr %9)
  call void @_ZN6String4dtorEv(ptr %30)
  call void @_ZN6String4dtorEv(ptr %29)
  %112 = load i32, ptr %result, align 4
  ret i32 %112
}

declare i32 @printf(ptr noundef, ...)

declare void @_ZN6String4ctorEPc(ptr, ptr)

declare %struct.String @_Z7op.plusRK6StringRK6String(ptr, ptr)

declare %struct.String @_Z7op.plusRK6StringRKPc(ptr, ptr)

declare %struct.String @_Z6op.mulsRK6String(i16, ptr)

declare %struct.String @_Z6op.mulRK6Stringi(ptr, i32)

declare void @_ZN6String4ctorEh(ptr, i8)

declare %struct.String @_Z6op.muliRK6String(i32, ptr)

declare i1 @_Z10isRawEqualPcPc(ptr, ptr)

declare i1 @_Z8op.equalRK6StringRK6String(ptr, ptr)

declare i1 @_Z11op.notequalRK6StringRK6String(ptr, ptr)

declare void @_Z12op.plusequalR6StringRKh(ptr, ptr)

declare void @_Z12op.plusequalR6StringRKPc(ptr, ptr)

declare void @_Z11op.mulequalR6Stringi(ptr, i32)

declare void @_ZN6String4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
