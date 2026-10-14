; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.String = type { ptr, i64, i64 }
%struct.Result = type { i32, %struct.Error }
%struct.Error = type { i32, ptr }

@anon.string.0 = private unnamed_addr constant [4 x i8] c"red\00", align 4
@anon.string.1 = private unnamed_addr constant [6 x i8] c"green\00", align 4
@anon.string.2 = private unnamed_addr constant [5 x i8] c"blue\00", align 4
@0 = private unnamed_addr constant [1 x i8] zeroinitializer, align 4
@anon.string.3 = private unnamed_addr constant [5 x i8] c"item\00", align 4
@anon.string.4 = private unnamed_addr constant [7 x i8] c"failed\00", align 4
@anon.string.5 = private unnamed_addr constant [15 x i8] c"limit too high\00", align 4
@anon.string.6 = private unnamed_addr constant [116 x i8] c"Program panicked at ./source.spice:46:9: %s\0A46  panic(Error(message.getRaw()));\0A    ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [11 x i8] c"Color: %s\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [9 x i8] c"Sum: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [16 x i8] c"Propagated: %d\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef ptr @_Z12getColorNamei(i32 noundef %0) #0 !dbg !9 {
  %color = alloca i32, align 4
  store i32 %0, ptr %color, align 4, !dbg !12
  %2 = load i32, ptr %color, align 4, !dbg !13
  switch i32 %2, label %switch.exit.L9 [
    i32 0, label %switch.case.L10
    i32 1, label %switch.case.L11
    i32 2, label %switch.case.L12
  ], !dbg !13

switch.case.L10:                                  ; preds = %1
  ret ptr @anon.string.0, !dbg !14

switch.case.L11:                                  ; preds = %1
  ret ptr @anon.string.1, !dbg !15

switch.case.L12:                                  ; preds = %1
  ret ptr @anon.string.2, !dbg !16

switch.exit.L9:                                   ; preds = %1
  ret ptr @0, !dbg !17
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z8sumUntili(i32 noundef %0) #0 !dbg !18 {
  %limit = alloca i32, align 4
  %sum = alloca i32, align 4
  %i = alloca i32, align 4
  %label = alloca %struct.String, align 8
  store i32 %0, ptr %limit, align 4, !dbg !19
  store i32 0, ptr %sum, align 4, !dbg !20
  store i32 0, ptr %i, align 4, !dbg !21
  br label %for.head.L18, !dbg !21

for.head.L18:                                     ; preds = %for.tail.L18, %1
  %2 = load i32, ptr %i, align 4, !dbg !22
  %3 = icmp slt i32 %2, 10, !dbg !22
  br i1 %3, label %for.body.L18, label %for.exit.L18, !dbg !22

for.body.L18:                                     ; preds = %for.head.L18
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %label, ptr noundef @anon.string.3), !dbg !23
  %4 = load i32, ptr %i, align 4, !dbg !24
  %5 = icmp eq i32 %4, 1, !dbg !24
  br i1 %5, label %if.then.L20, label %if.exit.L20, !dbg !24

if.then.L20:                                      ; preds = %for.body.L18
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %label), !dbg !25
  br label %for.tail.L18, !dbg !26

if.exit.L20:                                      ; preds = %for.body.L18
  %6 = load i32, ptr %limit, align 4, !dbg !27
  %7 = load i32, ptr %i, align 4, !dbg !27
  %8 = icmp eq i32 %7, %6, !dbg !27
  br i1 %8, label %if.then.L23, label %if.exit.L23, !dbg !27

if.then.L23:                                      ; preds = %if.exit.L20
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %label), !dbg !25
  br label %for.exit.L18, !dbg !28

if.exit.L23:                                      ; preds = %if.exit.L20
  %9 = call noundef i64 @_ZN6String9getLengthEv(ptr noundef nonnull align 8 dereferenceable(24) %label), !dbg !29
  %10 = trunc i64 %9 to i32, !dbg !29
  %11 = load i32, ptr %i, align 4, !dbg !29
  %12 = add nsw i32 %11, %10, !dbg !29
  %13 = load i32, ptr %sum, align 4, !dbg !30
  %14 = add nsw i32 %13, %12, !dbg !30
  store i32 %14, ptr %sum, align 4, !dbg !30
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %label), !dbg !25
  br label %for.tail.L18, !dbg !25

for.tail.L18:                                     ; preds = %if.exit.L23, %if.then.L20
  %15 = load i32, ptr %i, align 4, !dbg !31
  %16 = add nsw i32 %15, 1, !dbg !31
  store i32 %16, ptr %i, align 4, !dbg !31
  br label %for.head.L18, !dbg !31

for.exit.L18:                                     ; preds = %if.then.L23, %for.head.L18
  %17 = load i32, ptr %sum, align 4, !dbg !32
  ret i32 %17, !dbg !33
}

declare void @_ZN6String4ctorEPKc(ptr, ptr)

declare void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24))

declare i64 @_ZN6String9getLengthEv(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z9mightFailb(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8 %0, i1 noundef zeroext %1) #0 !dbg !34 {
  %doFail = alloca i1, align 1
  %3 = alloca %struct.Result, align 8
  %4 = alloca i32, align 4
  %5 = alloca %struct.Result, align 8
  store i1 %1, ptr %doFail, align 1, !dbg !35
  %6 = load i1, ptr %doFail, align 1, !dbg !36
  br i1 %6, label %if.then.L32, label %if.exit.L32, !dbg !36

if.then.L32:                                      ; preds = %2
  call void @_Z3errIiE6ResultIiEiPKc(ptr dead_on_unwind writable sret(%struct.Result) align 8 %3, i32 noundef 1, ptr noundef @anon.string.4), !dbg !37
  %7 = load %struct.Result, ptr %3, align 8, !dbg !37
  store %struct.Result %7, ptr %0, align 8, !dbg !38
  ret void, !dbg !38

if.exit.L32:                                      ; preds = %2
  store i32 3, ptr %4, align 4, !dbg !39
  call void @_Z2okIiE6ResultIiERKi(ptr dead_on_unwind writable sret(%struct.Result) align 8 %5, ptr noundef %4), !dbg !39
  %8 = load %struct.Result, ptr %5, align 8, !dbg !39
  store %struct.Result %8, ptr %0, align 8, !dbg !40
  ret void, !dbg !40
}

declare void @_Z3errIiE6ResultIiEiPKc(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, i32, ptr)

declare void @_Z2okIiE6ResultIiERKi(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z9propagateb(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8 %0, i1 noundef zeroext %1) #0 !dbg !41 {
  %doFail = alloca i1, align 1
  %3 = alloca %struct.Result, align 8
  %value = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca %struct.Result, align 8
  store i1 %1, ptr %doFail, align 1, !dbg !42
  %6 = load i1, ptr %doFail, align 1, !dbg !43
  call void @_Z9mightFailb(ptr dead_on_unwind writable sret(%struct.Result) align 8 %3, i1 noundef zeroext %6), !dbg !43
  %7 = call i1 @_ZN6ResultIiE5isErrEv(ptr %3), !dbg !43
  br i1 %7, label %err.prop.then.L39, label %err.prop.exit.L39, !dbg !43, !prof !44

err.prop.then.L39:                                ; preds = %2
  %8 = call ptr @_ZN6ResultIiE6getErrEv(ptr %3), !dbg !43
  call void @_Z3errIiE6ResultIiERK5Error(ptr dead_on_unwind writable sret(%struct.Result) align 8 %0, ptr %8), !dbg !43
  ret void, !dbg !45

err.prop.exit.L39:                                ; preds = %2
  %9 = call ptr @_ZN6ResultIiE6unwrapEv(ptr %3), !dbg !45
  %10 = load i32, ptr %9, align 4, !dbg !45
  store i32 %10, ptr %value, align 4, !dbg !45
  %11 = load i32, ptr %value, align 4, !dbg !46
  %12 = mul nsw i32 %11, 2, !dbg !46
  store i32 %12, ptr %4, align 4, !dbg !46
  call void @_Z2okIiE6ResultIiERKi(ptr dead_on_unwind writable sret(%struct.Result) align 8 %5, ptr noundef %4), !dbg !46
  %13 = load %struct.Result, ptr %5, align 8, !dbg !46
  store %struct.Result %13, ptr %0, align 8, !dbg !47
  ret void, !dbg !47
}

declare i1 @_ZN6ResultIiE5isErrEv(ptr)

declare ptr @_ZN6ResultIiE6getErrEv(ptr)

declare void @_Z3errIiE6ResultIiERK5Error(ptr dead_on_unwind noalias writable sret(%struct.Result) align 8, ptr)

declare ptr @_ZN6ResultIiE6unwrapEv(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z10checkLimiti(i32 noundef %0) #0 !dbg !48 {
  %limit = alloca i32, align 4
  %message = alloca %struct.String, align 8
  %2 = alloca %struct.Error, align 8
  store i32 %0, ptr %limit, align 4, !dbg !49
  %3 = load i32, ptr %limit, align 4, !dbg !50
  %4 = icmp sgt i32 %3, 10, !dbg !50
  br i1 %4, label %if.then.L44, label %if.exit.L44, !dbg !50

if.then.L44:                                      ; preds = %1
  call void @_ZN6String4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(24) %message, ptr noundef @anon.string.5), !dbg !51
  %5 = call ptr @__acrt_iob_func(i32 2), !dbg !52
  %6 = call noundef ptr @_ZN6String6getRawEv(ptr noundef nonnull align 8 dereferenceable(24) %message), !dbg !53
  call void @_ZN5Error4ctorEPKc(ptr noundef nonnull align 8 dereferenceable(16) %2, ptr noundef %6), !dbg !53
  %7 = getelementptr inbounds nuw %struct.Error, ptr %2, i32 0, i32 1, !dbg !53
  %8 = load ptr, ptr %7, align 8, !dbg !53
  %9 = call i32 (ptr, ptr, ...) @fprintf(ptr %5, ptr @anon.string.6, ptr %8), !dbg !53
  call void @_Z13sErrTraceDumpv(), !dbg !53
  call void @_ZN6String4dtorEv(ptr noundef nonnull align 8 dereferenceable(24) %message), !dbg !54
  call void @exit(i32 1), !dbg !52
  unreachable, !dbg !52

if.exit.L44:                                      ; preds = %1
  ret void, !dbg !55
}

; Function Attrs: nounwind
declare noundef ptr @__acrt_iob_func(i32 noundef) #1

declare ptr @_ZN6String6getRawEv(ptr)

declare void @_ZN5Error4ctorEPKc(ptr, ptr)

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

declare void @_Z13sErrTraceDumpv()

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #4 !dbg !56 {
  %propagated = alloca %struct.Result, align 8
  call void @_Z10checkLimiti(i32 noundef 4), !dbg !57
  %1 = call noundef ptr @_Z12getColorNamei(i32 noundef 1), !dbg !58
  %2 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, ptr noundef %1), !dbg !58
  %3 = call noundef i32 @_Z8sumUntili(i32 noundef 4), !dbg !59
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %3), !dbg !59
  call void @_Z9propagateb(ptr dead_on_unwind writable sret(%struct.Result) align 8 %propagated, i1 noundef zeroext false), !dbg !60
  %5 = call noundef ptr @_ZN6ResultIiE6unwrapEv(ptr noundef nonnull align 8 dereferenceable(24) %propagated), !dbg !61
  %6 = load i32, ptr %5, align 4, !dbg !61
  %7 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %6), !dbg !61
  ret i32 0, !dbg !62
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nounwind }
attributes #2 = { nofree }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #5 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.ident = !{!6}
!llvm.dbg.cu = !{!7}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!7 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !8, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: LineTablesOnly, splitDebugInlining: false)
!8 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-jumps")
!9 = distinct !DISubprogram(name: "getColorName", linkageName: "_Z12getColorNamei", scope: !8, file: !8, line: 8, type: !10, scopeLine: 8, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!10 = !DISubroutineType(types: !11)
!11 = !{}
!12 = !DILocation(line: 8, column: 24, scope: !9)
!13 = !DILocation(line: 9, column: 12, scope: !9)
!14 = !DILocation(line: 10, column: 28, scope: !9)
!15 = !DILocation(line: 11, column: 30, scope: !9)
!16 = !DILocation(line: 12, column: 29, scope: !9)
!17 = !DILocation(line: 14, column: 1, scope: !9)
!18 = distinct !DISubprogram(name: "sumUntil", linkageName: "_Z8sumUntili", scope: !8, file: !8, line: 16, type: !10, scopeLine: 16, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!19 = !DILocation(line: 16, column: 17, scope: !18)
!20 = !DILocation(line: 17, column: 15, scope: !18)
!21 = !DILocation(line: 18, column: 17, scope: !18)
!22 = !DILocation(line: 18, column: 24, scope: !18)
!23 = !DILocation(line: 19, column: 31, scope: !18)
!24 = !DILocation(line: 20, column: 17, scope: !18)
!25 = !DILocation(line: 27, column: 5, scope: !18)
!26 = !DILocation(line: 21, column: 13, scope: !18)
!27 = !DILocation(line: 23, column: 17, scope: !18)
!28 = !DILocation(line: 24, column: 13, scope: !18)
!29 = !DILocation(line: 26, column: 30, scope: !18)
!30 = !DILocation(line: 26, column: 9, scope: !18)
!31 = !DILocation(line: 18, column: 28, scope: !18)
!32 = !DILocation(line: 28, column: 12, scope: !18)
!33 = !DILocation(line: 28, column: 5, scope: !18)
!34 = distinct !DISubprogram(name: "mightFail", linkageName: "_Z9mightFailb", scope: !8, file: !8, line: 31, type: !10, scopeLine: 31, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!35 = !DILocation(line: 31, column: 26, scope: !34)
!36 = !DILocation(line: 32, column: 8, scope: !34)
!37 = !DILocation(line: 33, column: 28, scope: !34)
!38 = !DILocation(line: 33, column: 9, scope: !34)
!39 = !DILocation(line: 35, column: 20, scope: !34)
!40 = !DILocation(line: 35, column: 5, scope: !34)
!41 = distinct !DISubprogram(name: "propagate", linkageName: "_Z9propagateb", scope: !8, file: !8, line: 38, type: !10, scopeLine: 38, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!42 = !DILocation(line: 38, column: 26, scope: !41)
!43 = !DILocation(line: 39, column: 27, scope: !41)
!44 = !{!"branch_weights", i32 1, i32 1048575}
!45 = !DILocation(line: 39, column: 17, scope: !41)
!46 = !DILocation(line: 40, column: 28, scope: !41)
!47 = !DILocation(line: 40, column: 5, scope: !41)
!48 = distinct !DISubprogram(name: "checkLimit", linkageName: "_Z10checkLimiti", scope: !8, file: !8, line: 43, type: !10, scopeLine: 43, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!49 = !DILocation(line: 43, column: 14, scope: !48)
!50 = !DILocation(line: 44, column: 16, scope: !48)
!51 = !DILocation(line: 45, column: 33, scope: !48)
!52 = !DILocation(line: 46, column: 9, scope: !48)
!53 = !DILocation(line: 46, column: 21, scope: !48)
!54 = !DILocation(line: 47, column: 5, scope: !48)
!55 = !DILocation(line: 48, column: 1, scope: !48)
!56 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !8, file: !8, line: 50, type: !10, scopeLine: 50, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !7)
!57 = !DILocation(line: 51, column: 16, scope: !56)
!58 = !DILocation(line: 52, column: 40, scope: !56)
!59 = !DILocation(line: 53, column: 34, scope: !56)
!60 = !DILocation(line: 54, column: 40, scope: !56)
!61 = !DILocation(line: 55, column: 32, scope: !56)
!62 = !DILocation(line: 56, column: 1, scope: !56)
