; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [2 x i8] c".\00", align 1

define i32 @main() {
entry.l4:
  %result = alloca i32, align 4
  %q = alloca i64, align 8
  %q_new = alloca i64, align 8
  %r = alloca i64, align 8
  %r_new = alloca i64, align 8
  %t = alloca i64, align 8
  %t_new = alloca i64, align 8
  %k = alloca i64, align 8
  %k_new = alloca i64, align 8
  %m = alloca i64, align 8
  %m_new = alloca i64, align 8
  %x = alloca i64, align 8
  %x_new = alloca i64, align 8
  %iterations = alloca i32, align 4
  %printedDigits = alloca i32, align 4
  %i = alloca i32, align 4
  %0 = alloca i64, align 8
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i64, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca i64, align 8
  %26 = alloca i64, align 8
  %27 = alloca i64, align 8
  %28 = alloca i64, align 8
  %29 = alloca i32, align 4
  %30 = alloca i1, align 1
  store i32 0, i32* %result, align 4
  store i64 1, i64* %q, align 8
  store i64 0, i64* %q_new, align 8
  store i64 0, i64* %r, align 8
  store i64 0, i64* %r_new, align 8
  store i64 1, i64* %t, align 8
  store i64 0, i64* %t_new, align 8
  store i64 1, i64* %k, align 8
  store i64 0, i64* %k_new, align 8
  store i64 3, i64* %m, align 8
  store i64 0, i64* %m_new, align 8
  store i64 3, i64* %x, align 8
  store i64 0, i64* %x_new, align 8
  store i32 20, i32* %iterations, align 4
  store i32 0, i32* %printedDigits, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond.l21

for.l21:                                          ; preds = %for.cond.l21
  %31 = load i64, i64* %q, align 8
  %32 = mul i64 4, %31
  store i64 %32, i64* %0, align 8
  %33 = load i64, i64* %0, align 8
  %34 = load i64, i64* %r, align 8
  %35 = add i64 %33, %34
  %36 = load i64, i64* %t, align 8
  %37 = sub i64 %35, %36
  store i64 %37, i64* %1, align 8
  %38 = load i64, i64* %1, align 8
  %39 = load i64, i64* %m, align 8
  %40 = load i64, i64* %t, align 8
  %41 = mul i64 %39, %40
  store i64 %41, i64* %2, align 8
  %42 = load i64, i64* %2, align 8
  %43 = icmp slt i64 %38, %42
  store i1 %43, i1* %3, align 1
  %44 = load i1, i1* %3, align 1
  br i1 %44, label %if.then.l22, label %if.else.l22

if.then.l22:                                      ; preds = %for.l21
  %45 = load i64, i64* %m, align 8
  %46 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @0, i32 0, i32 0), i64 %45)
  %47 = load i32, i32* %printedDigits, align 4
  %48 = icmp eq i32 %47, 0
  store i1 %48, i1* %4, align 1
  %49 = load i1, i1* %4, align 1
  br i1 %49, label %if.then.l24, label %if.end.l24

if.then.l24:                                      ; preds = %if.then.l22
  %50 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @1, i32 0, i32 0))
  br label %if.end.l24

if.end.l24:                                       ; preds = %if.then.l24, %if.then.l22
  %51 = load i32, i32* %printedDigits, align 4
  %52 = add i32 %51, 1
  store i32 %52, i32* %printedDigits, align 4
  store i32 %51, i32* %5, align 4
  %53 = load i64, i64* %q, align 8
  %54 = mul i64 10, %53
  store i64 %54, i64* %6, align 8
  %55 = load i64, i64* %6, align 8
  store i64 %55, i64* %q_new, align 8
  %56 = load i64, i64* %r, align 8
  %57 = load i64, i64* %m, align 8
  %58 = load i64, i64* %t, align 8
  %59 = mul i64 %57, %58
  store i64 %59, i64* %7, align 8
  %60 = load i64, i64* %7, align 8
  %61 = sub i64 %56, %60
  store i64 %61, i64* %8, align 8
  %62 = load i64, i64* %8, align 8
  %63 = mul i64 10, %62
  store i64 %63, i64* %9, align 8
  %64 = load i64, i64* %9, align 8
  store i64 %64, i64* %r_new, align 8
  %65 = load i64, i64* %q, align 8
  %66 = mul i64 3, %65
  store i64 %66, i64* %10, align 8
  %67 = load i64, i64* %10, align 8
  %68 = load i64, i64* %r, align 8
  %69 = add i64 %67, %68
  store i64 %69, i64* %11, align 8
  %70 = load i64, i64* %11, align 8
  %71 = mul i64 10, %70
  store i64 %71, i64* %12, align 8
  %72 = load i64, i64* %12, align 8
  %73 = load i64, i64* %t, align 8
  %74 = sdiv i64 %72, %73
  store i64 %74, i64* %13, align 8
  %75 = load i64, i64* %13, align 8
  %76 = load i64, i64* %m, align 8
  %77 = mul i64 10, %76
  store i64 %77, i64* %14, align 8
  %78 = load i64, i64* %14, align 8
  %79 = sub i64 %75, %78
  store i64 %79, i64* %15, align 8
  %80 = load i64, i64* %15, align 8
  store i64 %80, i64* %m, align 8
  %81 = load i64, i64* %q_new, align 8
  store i64 %81, i64* %q, align 8
  %82 = load i64, i64* %r_new, align 8
  store i64 %82, i64* %r, align 8
  br label %if.end.l22

if.else.l22:                                      ; preds = %for.l21
  %83 = load i64, i64* %q, align 8
  %84 = load i64, i64* %k, align 8
  %85 = mul i64 %83, %84
  store i64 %85, i64* %16, align 8
  %86 = load i64, i64* %16, align 8
  store i64 %86, i64* %q_new, align 8
  %87 = load i64, i64* %q, align 8
  %88 = mul i64 2, %87
  store i64 %88, i64* %17, align 8
  %89 = load i64, i64* %17, align 8
  %90 = load i64, i64* %r, align 8
  %91 = add i64 %89, %90
  store i64 %91, i64* %18, align 8
  %92 = load i64, i64* %18, align 8
  %93 = load i64, i64* %x, align 8
  %94 = mul i64 %92, %93
  store i64 %94, i64* %19, align 8
  %95 = load i64, i64* %19, align 8
  store i64 %95, i64* %r_new, align 8
  %96 = load i64, i64* %t, align 8
  %97 = load i64, i64* %x, align 8
  %98 = mul i64 %96, %97
  store i64 %98, i64* %20, align 8
  %99 = load i64, i64* %20, align 8
  store i64 %99, i64* %t_new, align 8
  %100 = load i64, i64* %k, align 8
  %101 = add i64 %100, 1
  store i64 %101, i64* %21, align 8
  %102 = load i64, i64* %21, align 8
  store i64 %102, i64* %k_new, align 8
  %103 = load i64, i64* %q, align 8
  %104 = load i64, i64* %k, align 8
  %105 = mul i64 7, %104
  store i64 %105, i64* %22, align 8
  %106 = load i64, i64* %22, align 8
  %107 = add i64 %106, 2
  store i64 %107, i64* %23, align 8
  %108 = load i64, i64* %23, align 8
  %109 = mul i64 %103, %108
  store i64 %109, i64* %24, align 8
  %110 = load i64, i64* %24, align 8
  %111 = load i64, i64* %r, align 8
  %112 = load i64, i64* %x, align 8
  %113 = mul i64 %111, %112
  store i64 %113, i64* %25, align 8
  %114 = load i64, i64* %25, align 8
  %115 = add i64 %110, %114
  store i64 %115, i64* %26, align 8
  %116 = load i64, i64* %26, align 8
  %117 = load i64, i64* %t, align 8
  %118 = load i64, i64* %x, align 8
  %119 = mul i64 %117, %118
  store i64 %119, i64* %27, align 8
  %120 = load i64, i64* %27, align 8
  %121 = sdiv i64 %116, %120
  store i64 %121, i64* %28, align 8
  %122 = load i64, i64* %28, align 8
  store i64 %122, i64* %m, align 8
  %123 = load i64, i64* %x, align 8
  %124 = add i64 %123, 2
  store i64 %124, i64* %x, align 8
  %125 = load i64, i64* %q_new, align 8
  store i64 %125, i64* %q, align 8
  %126 = load i64, i64* %r_new, align 8
  store i64 %126, i64* %r, align 8
  %127 = load i64, i64* %t_new, align 8
  store i64 %127, i64* %t, align 8
  %128 = load i64, i64* %k_new, align 8
  store i64 %128, i64* %k, align 8
  br label %if.end.l22

if.end.l22:                                       ; preds = %if.else.l22, %if.end.l24
  br label %for.inc.l21

for.inc.l21:                                      ; preds = %if.end.l22
  %129 = load i32, i32* %i, align 4
  %130 = add i32 %129, 1
  store i32 %130, i32* %i, align 4
  store i32 %129, i32* %29, align 4
  br label %for.cond.l21

for.cond.l21:                                     ; preds = %for.inc.l21, %entry.l4
  %131 = load i32, i32* %i, align 4
  %132 = load i32, i32* %iterations, align 4
  %133 = icmp slt i32 %131, %132
  store i1 %133, i1* %30, align 1
  %134 = load i1, i1* %30, align 1
  br i1 %134, label %for.l21, label %for.end.l21

for.end.l21:                                      ; preds = %for.cond.l21
  %135 = load i32, i32* %result, align 4
  ret i32 %135
}

declare i32 @printf(i8*, ...)
