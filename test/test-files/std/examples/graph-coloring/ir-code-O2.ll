; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

@printf.str.1 = private unnamed_addr constant [5 x i8] c" %d \00", align 1
@printf.str.3 = private unnamed_addr constant [24 x i8] c"Solution does not exist\00", align 1
@str = private unnamed_addr constant [52 x i8] c"Solution Exists:\0AFollowing are the assigned colors:\00", align 1

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #0

; Function Attrs: nofree nounwind
define private fastcc i1 @_Z13graphColoringA4A4biiPi(i32 %0, ptr nocapture %1) unnamed_addr #0 {
  %3 = icmp eq i32 %0, 4
  br i1 %3, label %if.then.L29, label %for.head.L38.preheader

for.head.L38.preheader:                           ; preds = %2
  %4 = sext i32 %0 to i64
  %5 = getelementptr inbounds i32, ptr %1, i64 %4
  %6 = add nuw nsw i32 %0, 1
  store i32 1, ptr %5, align 4
  %7 = tail call fastcc i1 @_Z13graphColoringA4A4biiPi(i32 %6, ptr %1)
  br i1 %7, label %common.ret, label %if.exit.L42

if.then.L29:                                      ; preds = %2
  %8 = getelementptr inbounds i32, ptr %1, i64 1
  %9 = load i32, ptr %1, align 4
  %10 = load i32, ptr %8, align 4
  %11 = icmp eq i32 %10, %9
  br i1 %11, label %common.ret, label %land.1.L14C16.129.i

land.1.L14C16.129.i:                              ; preds = %if.then.L29
  %12 = getelementptr inbounds i32, ptr %1, i64 2
  %13 = load i32, ptr %12, align 4
  %14 = icmp eq i32 %13, %9
  br i1 %14, label %common.ret, label %land.1.L14C16.232.i

land.1.L14C16.232.i:                              ; preds = %land.1.L14C16.129.i
  %15 = getelementptr inbounds i32, ptr %1, i64 3
  %16 = load i32, ptr %15, align 4
  %17 = icmp eq i32 %16, %9
  %18 = icmp eq i32 %13, %10
  %or.cond = or i1 %18, %17
  %19 = icmp eq i32 %16, %13
  %or.cond76 = or i1 %19, %or.cond
  br i1 %or.cond76, label %common.ret, label %if.then.L31

common.ret:                                       ; preds = %for.head.L38.preheader, %if.exit.L42, %if.exit.L42.1, %if.exit.L42.2, %land.1.L14C16.232.i, %land.1.L14C16.129.i, %if.then.L29, %if.then.L31
  %common.ret.op = phi i1 [ true, %if.then.L31 ], [ false, %if.then.L29 ], [ false, %land.1.L14C16.129.i ], [ false, %land.1.L14C16.232.i ], [ true, %for.head.L38.preheader ], [ true, %if.exit.L42 ], [ true, %if.exit.L42.1 ], [ false, %if.exit.L42.2 ]
  ret i1 %common.ret.op

if.then.L31:                                      ; preds = %land.1.L14C16.232.i
  %puts.i = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  %20 = load i32, ptr %1, align 4
  %21 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %20)
  %22 = load i32, ptr %8, align 4
  %23 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %22)
  %24 = load i32, ptr %12, align 4
  %25 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %24)
  %26 = load i32, ptr %15, align 4
  %27 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.1, i32 %26)
  %putchar.i = tail call i32 @putchar(i32 10)
  br label %common.ret

if.exit.L42:                                      ; preds = %for.head.L38.preheader
  store i32 2, ptr %5, align 4
  %28 = tail call fastcc i1 @_Z13graphColoringA4A4biiPi(i32 %6, ptr nonnull %1)
  br i1 %28, label %common.ret, label %if.exit.L42.1

if.exit.L42.1:                                    ; preds = %if.exit.L42
  store i32 3, ptr %5, align 4
  %29 = tail call fastcc i1 @_Z13graphColoringA4A4biiPi(i32 %6, ptr nonnull %1)
  br i1 %29, label %common.ret, label %if.exit.L42.2

if.exit.L42.2:                                    ; preds = %if.exit.L42.1
  store i32 0, ptr %5, align 4
  br label %common.ret
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #1 {
for.exit.L71:
  %color = alloca [4 x i32], align 4
  call void @llvm.memset.p0.i64(ptr noundef nonnull align 4 dereferenceable(16) %color, i8 0, i64 16, i1 false)
  %0 = call fastcc i1 @_Z13graphColoringA4A4biiPi(i32 0, ptr nonnull %color) #3
  br i1 %0, label %if.exit.L75, label %if.then.L75

if.then.L75:                                      ; preds = %for.exit.L71
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.3)
  br label %if.exit.L75

if.exit.L75:                                      ; preds = %if.then.L75, %for.exit.L71
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr nocapture noundef readonly) local_unnamed_addr #0

; Function Attrs: nofree nounwind
declare noundef i32 @putchar(i32 noundef) local_unnamed_addr #0

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: write)
declare void @llvm.memset.p0.i64(ptr nocapture writeonly, i8, i64, i1 immarg) #2

attributes #0 = { nofree nounwind }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nocallback nofree nounwind willreturn memory(argmem: write) }
attributes #3 = { nounwind }
