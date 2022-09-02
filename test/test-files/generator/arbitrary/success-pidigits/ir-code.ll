; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@1 = private unnamed_addr constant [2 x i8] c".\00", align 1

define i32 @main() {
entry.l4:
  %result = alloca i32, align 4
  store i32 0, ptr %result, align 4
  %q = alloca i64, align 8
  store i64 1, ptr %q, align 8
  %q_new = alloca i64, align 8
  store i64 0, ptr %q_new, align 8
  %r = alloca i64, align 8
  store i64 0, ptr %r, align 8
  %r_new = alloca i64, align 8
  store i64 0, ptr %r_new, align 8
  %t = alloca i64, align 8
  store i64 1, ptr %t, align 8
  %t_new = alloca i64, align 8
  store i64 0, ptr %t_new, align 8
  %k = alloca i64, align 8
  store i64 1, ptr %k, align 8
  %k_new = alloca i64, align 8
  store i64 0, ptr %k_new, align 8
  %m = alloca i64, align 8
  store i64 3, ptr %m, align 8
  %m_new = alloca i64, align 8
  store i64 0, ptr %m_new, align 8
  %x = alloca i64, align 8
  store i64 3, ptr %x, align 8
  %x_new = alloca i64, align 8
  store i64 0, ptr %x_new, align 8
  %iterations = alloca i32, align 4
  store i32 20, ptr %iterations, align 4
  %printedDigits = alloca i32, align 4
  store i32 0, ptr %printedDigits, align 4
  %i = alloca i32, align 4
  store i32 0, ptr %i, align 4
  br label %for.cond.l21

for.l21:                                          ; preds = %for.cond.l21
  %0 = load i64, ptr %q, align 8
  %1 = mul i64 4, %0
  %2 = alloca i64, align 8
  store i64 %1, ptr %2, align 8
  %3 = load i64, ptr %2, align 8
  %4 = load i64, ptr %r, align 8
  %5 = add i64 %3, %4
  %6 = load i64, ptr %t, align 8
  %7 = sub i64 %5, %6
  %8 = alloca i64, align 8
  store i64 %7, ptr %8, align 8
  %9 = load i64, ptr %8, align 8
  %10 = load i64, ptr %m, align 8
  %11 = load i64, ptr %t, align 8
  %12 = mul i64 %10, %11
  %13 = alloca i64, align 8
  store i64 %12, ptr %13, align 8
  %14 = load i64, ptr %13, align 8
  %15 = icmp slt i64 %9, %14
  %16 = alloca i1, align 1
  store i1 %15, ptr %16, align 1
  %17 = load i1, ptr %16, align 1
  br i1 %17, label %if.then.l22, label %if.else.l22

if.then.l22:                                      ; preds = %for.l21
  %18 = load i64, ptr %m, align 8
  %19 = call i32 (ptr, ...) @printf(ptr @0, i64 %18)
  %20 = load i32, ptr %printedDigits, align 4
  %21 = icmp eq i32 %20, 0
  %22 = alloca i1, align 1
  store i1 %21, ptr %22, align 1
  %23 = load i1, ptr %22, align 1
  br i1 %23, label %if.then.l24, label %if.end.l24

if.then.l24:                                      ; preds = %if.then.l22
  %24 = call i32 (ptr, ...) @printf(ptr @1)
  br label %if.end.l24

if.end.l24:                                       ; preds = %if.then.l24, %if.then.l22
  %25 = load i32, ptr %printedDigits, align 4
  %26 = add i32 %25, 1
  store i32 %26, ptr %printedDigits, align 4
  %27 = alloca i32, align 4
  store i32 %25, ptr %27, align 4
  %28 = load i64, ptr %q, align 8
  %29 = mul i64 10, %28
  %30 = alloca i64, align 8
  store i64 %29, ptr %30, align 8
  %31 = load i64, ptr %30, align 8
  store i64 %31, ptr %q_new, align 8
  %32 = load i64, ptr %r, align 8
  %33 = load i64, ptr %m, align 8
  %34 = load i64, ptr %t, align 8
  %35 = mul i64 %33, %34
  %36 = alloca i64, align 8
  store i64 %35, ptr %36, align 8
  %37 = load i64, ptr %36, align 8
  %38 = sub i64 %32, %37
  %39 = alloca i64, align 8
  store i64 %38, ptr %39, align 8
  %40 = load i64, ptr %39, align 8
  %41 = mul i64 10, %40
  %42 = alloca i64, align 8
  store i64 %41, ptr %42, align 8
  %43 = load i64, ptr %42, align 8
  store i64 %43, ptr %r_new, align 8
  %44 = load i64, ptr %q, align 8
  %45 = mul i64 3, %44
  %46 = alloca i64, align 8
  store i64 %45, ptr %46, align 8
  %47 = load i64, ptr %46, align 8
  %48 = load i64, ptr %r, align 8
  %49 = add i64 %47, %48
  %50 = alloca i64, align 8
  store i64 %49, ptr %50, align 8
  %51 = load i64, ptr %50, align 8
  %52 = mul i64 10, %51
  %53 = alloca i64, align 8
  store i64 %52, ptr %53, align 8
  %54 = load i64, ptr %53, align 8
  %55 = load i64, ptr %t, align 8
  %56 = sdiv i64 %54, %55
  %57 = alloca i64, align 8
  store i64 %56, ptr %57, align 8
  %58 = load i64, ptr %57, align 8
  %59 = load i64, ptr %m, align 8
  %60 = mul i64 10, %59
  %61 = alloca i64, align 8
  store i64 %60, ptr %61, align 8
  %62 = load i64, ptr %61, align 8
  %63 = sub i64 %58, %62
  %64 = alloca i64, align 8
  store i64 %63, ptr %64, align 8
  %65 = load i64, ptr %64, align 8
  store i64 %65, ptr %m, align 8
  %66 = load i64, ptr %q_new, align 8
  store i64 %66, ptr %q, align 8
  %67 = load i64, ptr %r_new, align 8
  store i64 %67, ptr %r, align 8
  br label %if.end.l22

if.else.l22:                                      ; preds = %for.l21
  %68 = load i64, ptr %q, align 8
  %69 = load i64, ptr %k, align 8
  %70 = mul i64 %68, %69
  %71 = alloca i64, align 8
  store i64 %70, ptr %71, align 8
  %72 = load i64, ptr %71, align 8
  store i64 %72, ptr %q_new, align 8
  %73 = load i64, ptr %q, align 8
  %74 = mul i64 2, %73
  %75 = alloca i64, align 8
  store i64 %74, ptr %75, align 8
  %76 = load i64, ptr %75, align 8
  %77 = load i64, ptr %r, align 8
  %78 = add i64 %76, %77
  %79 = alloca i64, align 8
  store i64 %78, ptr %79, align 8
  %80 = load i64, ptr %79, align 8
  %81 = load i64, ptr %x, align 8
  %82 = mul i64 %80, %81
  %83 = alloca i64, align 8
  store i64 %82, ptr %83, align 8
  %84 = load i64, ptr %83, align 8
  store i64 %84, ptr %r_new, align 8
  %85 = load i64, ptr %t, align 8
  %86 = load i64, ptr %x, align 8
  %87 = mul i64 %85, %86
  %88 = alloca i64, align 8
  store i64 %87, ptr %88, align 8
  %89 = load i64, ptr %88, align 8
  store i64 %89, ptr %t_new, align 8
  %90 = load i64, ptr %k, align 8
  %91 = add i64 %90, 1
  %92 = alloca i64, align 8
  store i64 %91, ptr %92, align 8
  %93 = load i64, ptr %92, align 8
  store i64 %93, ptr %k_new, align 8
  %94 = load i64, ptr %q, align 8
  %95 = load i64, ptr %k, align 8
  %96 = mul i64 7, %95
  %97 = alloca i64, align 8
  store i64 %96, ptr %97, align 8
  %98 = load i64, ptr %97, align 8
  %99 = add i64 %98, 2
  %100 = alloca i64, align 8
  store i64 %99, ptr %100, align 8
  %101 = load i64, ptr %100, align 8
  %102 = mul i64 %94, %101
  %103 = alloca i64, align 8
  store i64 %102, ptr %103, align 8
  %104 = load i64, ptr %103, align 8
  %105 = load i64, ptr %r, align 8
  %106 = load i64, ptr %x, align 8
  %107 = mul i64 %105, %106
  %108 = alloca i64, align 8
  store i64 %107, ptr %108, align 8
  %109 = load i64, ptr %108, align 8
  %110 = add i64 %104, %109
  %111 = alloca i64, align 8
  store i64 %110, ptr %111, align 8
  %112 = load i64, ptr %111, align 8
  %113 = load i64, ptr %t, align 8
  %114 = load i64, ptr %x, align 8
  %115 = mul i64 %113, %114
  %116 = alloca i64, align 8
  store i64 %115, ptr %116, align 8
  %117 = load i64, ptr %116, align 8
  %118 = sdiv i64 %112, %117
  %119 = alloca i64, align 8
  store i64 %118, ptr %119, align 8
  %120 = load i64, ptr %119, align 8
  store i64 %120, ptr %m, align 8
  %121 = load i64, ptr %x, align 8
  %122 = add i64 %121, 2
  store i64 %122, ptr %x, align 8
  %123 = load i64, ptr %q_new, align 8
  store i64 %123, ptr %q, align 8
  %124 = load i64, ptr %r_new, align 8
  store i64 %124, ptr %r, align 8
  %125 = load i64, ptr %t_new, align 8
  store i64 %125, ptr %t, align 8
  %126 = load i64, ptr %k_new, align 8
  store i64 %126, ptr %k, align 8
  br label %if.end.l22

if.end.l22:                                       ; preds = %if.else.l22, %if.end.l24
  br label %for.inc.l21

for.inc.l21:                                      ; preds = %if.end.l22
  %127 = load i32, ptr %i, align 4
  %128 = add i32 %127, 1
  store i32 %128, ptr %i, align 4
  %129 = alloca i32, align 4
  store i32 %127, ptr %129, align 4
  br label %for.cond.l21

for.cond.l21:                                     ; preds = %for.inc.l21, %entry.l4
  %130 = load i32, ptr %i, align 4
  %131 = load i32, ptr %iterations, align 4
  %132 = icmp slt i32 %130, %131
  %133 = alloca i1, align 1
  store i1 %132, ptr %133, align 1
  %134 = load i1, ptr %133, align 1
  br i1 %134, label %for.l21, label %for.end.l21

for.end.l21:                                      ; preds = %for.cond.l21
  %135 = load i32, ptr %result, align 4
  ret i32 %135
}

declare i32 @printf(ptr, ...)
