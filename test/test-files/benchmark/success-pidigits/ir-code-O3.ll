; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@0 = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: nofree nounwind
define i32 @main() local_unnamed_addr #0 {
entry.l4:
  br label %for.l21

for.l21:                                          ; preds = %entry.l4, %for.inc.l21
  %q.185 = phi i64 [ 1, %entry.l4 ], [ %q.0, %for.inc.l21 ]
  %r.184 = phi i64 [ 0, %entry.l4 ], [ %r.0, %for.inc.l21 ]
  %t.183 = phi i64 [ 1, %entry.l4 ], [ %t.0, %for.inc.l21 ]
  %k.182 = phi i64 [ 1, %entry.l4 ], [ %k.0, %for.inc.l21 ]
  %m.181 = phi i64 [ 3, %entry.l4 ], [ %m.0, %for.inc.l21 ]
  %x.180 = phi i64 [ 3, %entry.l4 ], [ %x.0, %for.inc.l21 ]
  %printedDigits.179 = phi i32 [ 0, %entry.l4 ], [ %printedDigits.0, %for.inc.l21 ]
  %i.078 = phi i32 [ 0, %entry.l4 ], [ %29, %for.inc.l21 ]
  %0 = shl i64 %q.185, 2
  %1 = sub i64 %r.184, %t.183
  %2 = add i64 %1, %0
  %3 = mul i64 %t.183, %m.181
  %4 = icmp slt i64 %2, %3
  br i1 %4, label %if.then.l22, label %if.else.l22

if.then.l22:                                      ; preds = %for.l21
  %5 = tail call i32 (ptr, ...) @printf(ptr nonnull @0, i64 %m.181)
  %6 = icmp eq i32 %printedDigits.179, 0
  br i1 %6, label %if.then.l24, label %if.end.l24

if.then.l24:                                      ; preds = %if.then.l22
  %putchar = tail call i32 @putchar(i32 46)
  br label %if.end.l24

if.end.l24:                                       ; preds = %if.then.l24, %if.then.l22
  %7 = add i32 %printedDigits.179, 1
  %8 = mul i64 %q.185, 10
  %9 = sub i64 %r.184, %3
  %10 = mul i64 %9, 10
  %11 = mul i64 %q.185, 3
  %12 = add i64 %11, %r.184
  %13 = mul i64 %12, 10
  %14 = sdiv i64 %13, %t.183
  %.neg = mul i64 %m.181, -10
  %15 = add i64 %14, %.neg
  br label %for.inc.l21

if.else.l22:                                      ; preds = %for.l21
  %16 = mul i64 %q.185, %k.182
  %17 = shl i64 %q.185, 1
  %18 = add i64 %17, %r.184
  %19 = mul i64 %18, %x.180
  %20 = mul i64 %t.183, %x.180
  %21 = add i64 %k.182, 1
  %22 = mul i64 %k.182, 7
  %23 = add i64 %22, 2
  %24 = mul i64 %q.185, %23
  %25 = mul i64 %r.184, %x.180
  %26 = add i64 %24, %25
  %27 = sdiv i64 %26, %20
  %28 = add i64 %x.180, 2
  br label %for.inc.l21

for.inc.l21:                                      ; preds = %if.end.l24, %if.else.l22
  %printedDigits.0 = phi i32 [ %7, %if.end.l24 ], [ %printedDigits.179, %if.else.l22 ]
  %x.0 = phi i64 [ %x.180, %if.end.l24 ], [ %28, %if.else.l22 ]
  %m.0 = phi i64 [ %15, %if.end.l24 ], [ %27, %if.else.l22 ]
  %k.0 = phi i64 [ %k.182, %if.end.l24 ], [ %21, %if.else.l22 ]
  %t.0 = phi i64 [ %t.183, %if.end.l24 ], [ %20, %if.else.l22 ]
  %r.0 = phi i64 [ %10, %if.end.l24 ], [ %19, %if.else.l22 ]
  %q.0 = phi i64 [ %8, %if.end.l24 ], [ %16, %if.else.l22 ]
  %29 = add nuw nsw i32 %i.078, 1
  %exitcond.not = icmp eq i32 %29, 20
  br i1 %exitcond.not, label %for.end.l21, label %for.l21

for.end.l21:                                      ; preds = %for.inc.l21
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

attributes #0 = { nofree nounwind }