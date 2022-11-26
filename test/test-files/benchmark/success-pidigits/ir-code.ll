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
  %i1 = alloca i64, align 8
  %0 = alloca i64, align 8
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i32, align 4
  %q_new2 = alloca i64, align 8
  %6 = alloca i64, align 8
  %7 = alloca i64, align 8
  %8 = alloca i64, align 8
  %r_new3 = alloca i64, align 8
  %9 = alloca i64, align 8
  %10 = alloca i64, align 8
  %11 = alloca i64, align 8
  %m4 = alloca i64, align 8
  %12 = alloca i64, align 8
  %13 = alloca i64, align 8
  %m5 = alloca i64, align 8
  %m6 = alloca i64, align 8
  %14 = alloca i64, align 8
  %15 = alloca i64, align 8
  %16 = alloca i64, align 8
  %17 = alloca i64, align 8
  %m7 = alloca i64, align 8
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i64, align 8
  %r_new8 = alloca i64, align 8
  %21 = alloca i64, align 8
  %22 = alloca i64, align 8
  %r_new9 = alloca i64, align 8
  %23 = alloca i64, align 8
  %24 = alloca i64, align 8
  %25 = alloca i64, align 8
  %26 = alloca i64, align 8
  %27 = alloca i64, align 8
  %m10 = alloca i64, align 8
  %28 = alloca i64, align 8
  %29 = alloca i64, align 8
  %m11 = alloca i64, align 8
  %30 = alloca i64, align 8
  %31 = alloca i64, align 8
  %32 = alloca i64, align 8
  %33 = alloca i64, align 8
  %34 = alloca i64, align 8
  %35 = alloca i64, align 8
  %36 = alloca i64, align 8
  %37 = alloca i32, align 4
  %38 = alloca i1, align 1
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
  %39 = load i64, ptr %q, align 8
  store i64 4, ptr %i1, align 8
  %40 = mul i64 4, %39
  store i64 %40, ptr %0, align 8
  %41 = load i64, ptr %0, align 8
  %42 = load i64, ptr %r, align 8
  %43 = add i64 %41, %42
  %44 = load i64, ptr %t, align 8
  %45 = sub i64 %43, %44
  store i64 %45, ptr %1, align 8
  %46 = load i64, ptr %1, align 8
  %47 = load i64, ptr %m, align 8
  %48 = load i64, ptr %t, align 8
  %49 = mul i64 %47, %48
  store i64 %49, ptr %2, align 8
  %50 = load i64, ptr %2, align 8
  %51 = icmp slt i64 %46, %50
  store i1 %51, ptr %3, align 1
  %52 = load i1, ptr %3, align 1
  br i1 %52, label %if.then.l22, label %if.else.l22

if.then.l22:                                      ; preds = %for.l21
  %53 = load i64, ptr %m, align 8
  %54 = call i32 (ptr, ...) @printf(ptr @0, i64 %53)
  %55 = load i32, ptr %printedDigits, align 4
  %56 = icmp eq i32 %55, 0
  store i1 %56, ptr %4, align 1
  %57 = load i1, ptr %4, align 1
  br i1 %57, label %if.then.l24, label %if.end.l24

if.then.l24:                                      ; preds = %if.then.l22
  %58 = call i32 (ptr, ...) @printf(ptr @1)
  br label %if.end.l24

if.end.l24:                                       ; preds = %if.then.l24, %if.then.l22
  %59 = load i32, ptr %printedDigits, align 4
  %60 = add i32 %59, 1
  store i32 %60, ptr %printedDigits, align 4
  store i32 %59, ptr %5, align 4
  %61 = load i64, ptr %q, align 8
  store i64 10, ptr %q_new2, align 8
  %62 = mul i64 10, %61
  store i64 %62, ptr %6, align 8
  %63 = load i64, ptr %6, align 8
  store i64 %63, ptr %q_new, align 8
  %64 = load i64, ptr %r, align 8
  %65 = load i64, ptr %m, align 8
  %66 = load i64, ptr %t, align 8
  %67 = mul i64 %65, %66
  store i64 %67, ptr %7, align 8
  %68 = load i64, ptr %7, align 8
  %69 = sub i64 %64, %68
  store i64 %69, ptr %8, align 8
  %70 = load i64, ptr %8, align 8
  store i64 10, ptr %r_new3, align 8
  %71 = load i64, ptr %r, align 8
  %72 = load i64, ptr %m, align 8
  %73 = load i64, ptr %t, align 8
  %74 = mul i64 %72, %73
  store i64 %74, ptr %9, align 8
  %75 = load i64, ptr %9, align 8
  %76 = sub i64 %71, %75
  store i64 %76, ptr %10, align 8
  %77 = mul i64 10, %70
  store i64 %77, ptr %11, align 8
  %78 = load i64, ptr %11, align 8
  store i64 %78, ptr %r_new, align 8
  %79 = load i64, ptr %q, align 8
  store i64 3, ptr %m4, align 8
  %80 = mul i64 3, %79
  store i64 %80, ptr %12, align 8
  %81 = load i64, ptr %12, align 8
  %82 = load i64, ptr %r, align 8
  %83 = add i64 %81, %82
  store i64 %83, ptr %13, align 8
  %84 = load i64, ptr %13, align 8
  store i64 10, ptr %m5, align 8
  %85 = load i64, ptr %q, align 8
  store i64 3, ptr %m6, align 8
  %86 = mul i64 3, %85
  store i64 %86, ptr %14, align 8
  %87 = load i64, ptr %14, align 8
  %88 = load i64, ptr %r, align 8
  %89 = add i64 %87, %88
  store i64 %89, ptr %15, align 8
  %90 = mul i64 10, %84
  store i64 %90, ptr %16, align 8
  %91 = load i64, ptr %16, align 8
  %92 = load i64, ptr %t, align 8
  %93 = sdiv i64 %91, %92
  store i64 %93, ptr %17, align 8
  %94 = load i64, ptr %17, align 8
  %95 = load i64, ptr %m, align 8
  store i64 10, ptr %m7, align 8
  %96 = mul i64 10, %95
  store i64 %96, ptr %18, align 8
  %97 = load i64, ptr %18, align 8
  %98 = sub i64 %94, %97
  store i64 %98, ptr %19, align 8
  %99 = load i64, ptr %19, align 8
  store i64 %99, ptr %m, align 8
  %100 = load i64, ptr %q_new, align 8
  store i64 %100, ptr %q, align 8
  %101 = load i64, ptr %r_new, align 8
  store i64 %101, ptr %r, align 8
  br label %if.end.l22

if.else.l22:                                      ; preds = %for.l21
  %102 = load i64, ptr %q, align 8
  %103 = load i64, ptr %k, align 8
  %104 = mul i64 %102, %103
  store i64 %104, ptr %20, align 8
  %105 = load i64, ptr %20, align 8
  store i64 %105, ptr %q_new, align 8
  %106 = load i64, ptr %q, align 8
  store i64 2, ptr %r_new8, align 8
  %107 = mul i64 2, %106
  store i64 %107, ptr %21, align 8
  %108 = load i64, ptr %21, align 8
  %109 = load i64, ptr %r, align 8
  %110 = add i64 %108, %109
  store i64 %110, ptr %22, align 8
  %111 = load i64, ptr %22, align 8
  %112 = load i64, ptr %x, align 8
  %113 = load i64, ptr %q, align 8
  store i64 2, ptr %r_new9, align 8
  %114 = mul i64 2, %113
  store i64 %114, ptr %23, align 8
  %115 = load i64, ptr %23, align 8
  %116 = load i64, ptr %r, align 8
  %117 = add i64 %115, %116
  store i64 %117, ptr %24, align 8
  %118 = mul i64 %111, %112
  store i64 %118, ptr %25, align 8
  %119 = load i64, ptr %25, align 8
  store i64 %119, ptr %r_new, align 8
  %120 = load i64, ptr %t, align 8
  %121 = load i64, ptr %x, align 8
  %122 = mul i64 %120, %121
  store i64 %122, ptr %26, align 8
  %123 = load i64, ptr %26, align 8
  store i64 %123, ptr %t_new, align 8
  %124 = load i64, ptr %k, align 8
  %125 = add i64 %124, 1
  store i64 %125, ptr %27, align 8
  %126 = load i64, ptr %27, align 8
  store i64 %126, ptr %k_new, align 8
  %127 = load i64, ptr %q, align 8
  %128 = load i64, ptr %k, align 8
  store i64 7, ptr %m10, align 8
  %129 = mul i64 7, %128
  store i64 %129, ptr %28, align 8
  %130 = load i64, ptr %28, align 8
  %131 = add i64 %130, 2
  store i64 %131, ptr %29, align 8
  %132 = load i64, ptr %29, align 8
  %133 = load i64, ptr %k, align 8
  store i64 7, ptr %m11, align 8
  %134 = mul i64 7, %133
  store i64 %134, ptr %30, align 8
  %135 = load i64, ptr %30, align 8
  %136 = add i64 %135, 2
  store i64 %136, ptr %31, align 8
  %137 = mul i64 %127, %132
  store i64 %137, ptr %32, align 8
  %138 = load i64, ptr %32, align 8
  %139 = load i64, ptr %r, align 8
  %140 = load i64, ptr %x, align 8
  %141 = mul i64 %139, %140
  store i64 %141, ptr %33, align 8
  %142 = load i64, ptr %33, align 8
  %143 = add i64 %138, %142
  store i64 %143, ptr %34, align 8
  %144 = load i64, ptr %34, align 8
  %145 = load i64, ptr %t, align 8
  %146 = load i64, ptr %x, align 8
  %147 = mul i64 %145, %146
  store i64 %147, ptr %35, align 8
  %148 = load i64, ptr %35, align 8
  %149 = sdiv i64 %144, %148
  store i64 %149, ptr %36, align 8
  %150 = load i64, ptr %36, align 8
  store i64 %150, ptr %m, align 8
  %151 = load i64, ptr %x, align 8
  %152 = add i64 %151, 2
  store i64 %152, ptr %x, align 8
  %153 = load i64, ptr %q_new, align 8
  store i64 %153, ptr %q, align 8
  %154 = load i64, ptr %r_new, align 8
  store i64 %154, ptr %r, align 8
  %155 = load i64, ptr %t_new, align 8
  store i64 %155, ptr %t, align 8
  %156 = load i64, ptr %k_new, align 8
  store i64 %156, ptr %k, align 8
  br label %if.end.l22

if.end.l22:                                       ; preds = %if.else.l22, %if.end.l24
  br label %for.inc.l21

for.inc.l21:                                      ; preds = %if.end.l22
  %157 = load i32, ptr %i, align 4
  %158 = add i32 %157, 1
  store i32 %158, ptr %i, align 4
  store i32 %157, ptr %37, align 4
  br label %for.cond.l21

for.cond.l21:                                     ; preds = %for.inc.l21, %entry.l4
  %159 = load i32, ptr %i, align 4
  %160 = load i32, ptr %iterations, align 4
  %161 = icmp slt i32 %159, %160
  store i1 %161, ptr %38, align 1
  %162 = load i1, ptr %38, align 1
  br i1 %162, label %for.l21, label %for.end.l21

for.end.l21:                                      ; preds = %for.cond.l21
  %163 = load i32, ptr %result, align 4
  ret i32 %163
}

declare i32 @printf(ptr, ...)
