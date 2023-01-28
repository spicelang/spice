; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  br label %for.body.L21

for.body.L21:                                     ; preds = %0, %for.tail.L21
  %i.035 = phi i32 [ 0, %0 ], [ %30, %for.tail.L21 ]
  %printedDigits.034 = phi i32 [ 0, %0 ], [ %printedDigits.1, %for.tail.L21 ]
  %q.033 = phi i64 [ 1, %0 ], [ %q.1, %for.tail.L21 ]
  %x.032 = phi i64 [ 3, %0 ], [ %x.1, %for.tail.L21 ]
  %r.031 = phi i64 [ 0, %0 ], [ %r.1, %for.tail.L21 ]
  %m.030 = phi i64 [ 3, %0 ], [ %m.1, %for.tail.L21 ]
  %k.029 = phi i64 [ 1, %0 ], [ %k.1, %for.tail.L21 ]
  %t.028 = phi i64 [ 1, %0 ], [ %t.1, %for.tail.L21 ]
  %1 = shl i64 %q.033, 2
  %2 = add i64 %r.031, %1
  %3 = sub i64 %2, %t.028
  %4 = mul i64 %t.028, %m.030
  %5 = icmp slt i64 %3, %4
  br i1 %5, label %if.then.L22, label %if.else.L22

if.then.L22:                                      ; preds = %for.body.L21
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0, i64 %m.030)
  %7 = icmp eq i32 %printedDigits.034, 0
  br i1 %7, label %if.then.L24, label %if.exit.L24

if.then.L24:                                      ; preds = %if.then.L22
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.exit.L24

if.exit.L24:                                      ; preds = %if.then.L24, %if.then.L22
  %8 = add i32 %printedDigits.034, 1
  %9 = mul i64 %q.033, 10
  %10 = sub i64 %r.031, %4
  %11 = mul i64 %10, 10
  %12 = mul i64 %q.033, 3
  %13 = add i64 %r.031, %12
  %14 = mul i64 %13, 10
  %15 = sdiv i64 %14, %t.028
  %.neg = mul i64 %m.030, -10
  %16 = add i64 %15, %.neg
  br label %for.tail.L21

if.else.L22:                                      ; preds = %for.body.L21
  %17 = mul i64 %k.029, %q.033
  %18 = shl i64 %q.033, 1
  %19 = add i64 %r.031, %18
  %20 = mul i64 %19, %x.032
  %21 = mul i64 %t.028, %x.032
  %22 = add i64 %k.029, 1
  %23 = mul i64 %k.029, 7
  %24 = add i64 %23, 2
  %25 = mul i64 %24, %q.033
  %26 = mul i64 %r.031, %x.032
  %27 = add i64 %25, %26
  %28 = sdiv i64 %27, %21
  %29 = add i64 %x.032, 2
  br label %for.tail.L21

for.tail.L21:                                     ; preds = %if.exit.L24, %if.else.L22
  %t.1 = phi i64 [ %t.028, %if.exit.L24 ], [ %21, %if.else.L22 ]
  %k.1 = phi i64 [ %k.029, %if.exit.L24 ], [ %22, %if.else.L22 ]
  %m.1 = phi i64 [ %16, %if.exit.L24 ], [ %28, %if.else.L22 ]
  %r.1 = phi i64 [ %11, %if.exit.L24 ], [ %20, %if.else.L22 ]
  %x.1 = phi i64 [ %x.032, %if.exit.L24 ], [ %29, %if.else.L22 ]
  %q.1 = phi i64 [ %9, %if.exit.L24 ], [ %17, %if.else.L22 ]
  %printedDigits.1 = phi i32 [ %8, %if.exit.L24 ], [ %printedDigits.034, %if.else.L22 ]
  %30 = add nuw nsw i32 %i.035, 1
  %exitcond.not = icmp eq i32 %30, 20
  br i1 %exitcond.not, label %for.exit.L21, label %for.body.L21

for.exit.L21:                                     ; preds = %for.tail.L21
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
