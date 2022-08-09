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
  store i32 0, ptr %result, align 4
  store i64 1, ptr %q, align 8
  store i64 0, ptr %q_new, align 8
  store i64 0, ptr %r, align 8
  store i64 0, ptr %r_new, align 8
  store i64 1, ptr %t, align 8
  store i64 0, ptr %t_new, align 8
  store i64 1, ptr %k, align 8
  store i64 0, ptr %k_new, align 8
  store i64 3, ptr %m, align 8
  store i64 0, ptr %m_new, align 8
  store i64 3, ptr %x, align 8
  store i64 0, ptr %x_new, align 8
  store i32 20, ptr %iterations, align 4
  store i32 0, ptr %printedDigits, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l21

for.l21:                                          ; preds = %for.cond.l21
  %31 = load i64, ptr %q, align 8
  %32 = mul i64 4, %31
  store i64 %32, ptr %0, align 8
  %33 = load i64, ptr %0, align 8
  %34 = load i64, ptr %r, align 8
  %35 = add i64 %33, %34
  %36 = load i64, ptr %t, align 8
  %37 = sub i64 %35, %36
  store i64 %37, ptr %1, align 8
  %38 = load i64, ptr %1, align 8
  %39 = load i64, ptr %m, align 8
  %40 = load i64, ptr %t, align 8
  %41 = mul i64 %39, %40
  store i64 %41, ptr %2, align 8
  %42 = load i64, ptr %2, align 8
  %43 = icmp slt i64 %38, %42
  store i1 %43, ptr %3, align 1
  %44 = load i1, ptr %3, align 1
  br i1 %44, label %if.then.l22, label %if.else.l22

if.then.l22:                                      ; preds = %for.l21
  %45 = load i64, ptr %m, align 8
  %46 = call i32 (ptr, ...) @printf(ptr @0, i64 %45)
  %47 = load i32, ptr %printedDigits, align 4
  %48 = icmp eq i32 %47, 0
  store i1 %48, ptr %4, align 1
  %49 = load i1, ptr %4, align 1
  br i1 %49, label %if.then.l24, label %if.end.l24

if.then.l24:                                      ; preds = %if.then.l22
  %50 = call i32 (ptr, ...) @printf(ptr @1)
  br label %if.end.l24

if.end.l24:                                       ; preds = %if.then.l24, %if.then.l22
  %51 = load i32, ptr %printedDigits, align 4
  %52 = add i32 %51, 1
  store i32 %52, ptr %printedDigits, align 4
  store i32 %51, ptr %5, align 4
  %53 = load i64, ptr %q, align 8
  %54 = mul i64 10, %53
  store i64 %54, ptr %6, align 8
  %55 = load i64, ptr %6, align 8
  store i64 %55, ptr %q_new, align 8
  %56 = load i64, ptr %r, align 8
  %57 = load i64, ptr %m, align 8
  %58 = load i64, ptr %t, align 8
  %59 = mul i64 %57, %58
  store i64 %59, ptr %7, align 8
  %60 = load i64, ptr %7, align 8
  %61 = sub i64 %56, %60
  store i64 %61, ptr %8, align 8
  %62 = load i64, ptr %8, align 8
  %63 = mul i64 10, %62
  store i64 %63, ptr %9, align 8
  %64 = load i64, ptr %9, align 8
  store i64 %64, ptr %r_new, align 8
  %65 = load i64, ptr %q, align 8
  %66 = mul i64 3, %65
  store i64 %66, ptr %10, align 8
  %67 = load i64, ptr %10, align 8
  %68 = load i64, ptr %r, align 8
  %69 = add i64 %67, %68
  store i64 %69, ptr %11, align 8
  %70 = load i64, ptr %11, align 8
  %71 = mul i64 10, %70
  store i64 %71, ptr %12, align 8
  %72 = load i64, ptr %12, align 8
  %73 = load i64, ptr %t, align 8
  %74 = sdiv i64 %72, %73
  store i64 %74, ptr %13, align 8
  %75 = load i64, ptr %13, align 8
  %76 = load i64, ptr %m, align 8
  %77 = mul i64 10, %76
  store i64 %77, ptr %14, align 8
  %78 = load i64, ptr %14, align 8
  %79 = sub i64 %75, %78
  store i64 %79, ptr %15, align 8
  %80 = load i64, ptr %15, align 8
  store i64 %80, ptr %m, align 8
  %81 = load i64, ptr %q_new, align 8
  store i64 %81, ptr %q, align 8
  %82 = load i64, ptr %r_new, align 8
  store i64 %82, ptr %r, align 8
  br label %if.end.l22

if.else.l22:                                      ; preds = %for.l21
  %83 = load i64, ptr %q, align 8
  %84 = load i64, ptr %k, align 8
  %85 = mul i64 %83, %84
  store i64 %85, ptr %16, align 8
  %86 = load i64, ptr %16, align 8
  store i64 %86, ptr %q_new, align 8
  %87 = load i64, ptr %q, align 8
  %88 = mul i64 2, %87
  store i64 %88, ptr %17, align 8
  %89 = load i64, ptr %17, align 8
  %90 = load i64, ptr %r, align 8
  %91 = add i64 %89, %90
  store i64 %91, ptr %18, align 8
  %92 = load i64, ptr %18, align 8
  %93 = load i64, ptr %x, align 8
  %94 = mul i64 %92, %93
  store i64 %94, ptr %19, align 8
  %95 = load i64, ptr %19, align 8
  store i64 %95, ptr %r_new, align 8
  %96 = load i64, ptr %t, align 8
  %97 = load i64, ptr %x, align 8
  %98 = mul i64 %96, %97
  store i64 %98, ptr %20, align 8
  %99 = load i64, ptr %20, align 8
  store i64 %99, ptr %t_new, align 8
  %100 = load i64, ptr %k, align 8
  %101 = add i64 %100, 1
  store i64 %101, ptr %21, align 8
  %102 = load i64, ptr %21, align 8
  store i64 %102, ptr %k_new, align 8
  %103 = load i64, ptr %q, align 8
  %104 = load i64, ptr %k, align 8
  %105 = mul i64 7, %104
  store i64 %105, ptr %22, align 8
  %106 = load i64, ptr %22, align 8
  %107 = add i64 %106, 2
  store i64 %107, ptr %23, align 8
  %108 = load i64, ptr %23, align 8
  %109 = mul i64 %103, %108
  store i64 %109, ptr %24, align 8
  %110 = load i64, ptr %24, align 8
  %111 = load i64, ptr %r, align 8
  %112 = load i64, ptr %x, align 8
  %113 = mul i64 %111, %112
  store i64 %113, ptr %25, align 8
  %114 = load i64, ptr %25, align 8
  %115 = add i64 %110, %114
  store i64 %115, ptr %26, align 8
  %116 = load i64, ptr %26, align 8
  %117 = load i64, ptr %t, align 8
  %118 = load i64, ptr %x, align 8
  %119 = mul i64 %117, %118
  store i64 %119, ptr %27, align 8
  %120 = load i64, ptr %27, align 8
  %121 = sdiv i64 %116, %120
  store i64 %121, ptr %28, align 8
  %122 = load i64, ptr %28, align 8
  store i64 %122, ptr %m, align 8
  %123 = load i64, ptr %x, align 8
  %124 = add i64 %123, 2
  store i64 %124, ptr %x, align 8
  %125 = load i64, ptr %q_new, align 8
  store i64 %125, ptr %q, align 8
  %126 = load i64, ptr %r_new, align 8
  store i64 %126, ptr %r, align 8
  %127 = load i64, ptr %t_new, align 8
  store i64 %127, ptr %t, align 8
  %128 = load i64, ptr %k_new, align 8
  store i64 %128, ptr %k, align 8
  br label %if.end.l22

if.end.l22:                                       ; preds = %if.else.l22, %if.end.l24
  br label %for.inc.l21

for.inc.l21:                                      ; preds = %if.end.l22
  %129 = load i32, ptr %i, align 4
  %130 = add i32 %129, 1
  store i32 %130, ptr %i, align 4
  store i32 %129, ptr %29, align 4
  br label %for.cond.l21

for.cond.l21:                                     ; preds = %for.inc.l21, %entry.l4
  %131 = load i32, ptr %i, align 4
  %132 = load i32, ptr %iterations, align 4
  %133 = icmp slt i32 %131, %132
  store i1 %133, ptr %30, align 1
  %134 = load i1, ptr %30, align 1
  br i1 %134, label %for.l21, label %for.end.l21

for.end.l21:                                      ; preds = %for.cond.l21
  %135 = load i32, ptr %result, align 4
  ret i32 %135
}

declare i32 @printf(ptr, ...)
