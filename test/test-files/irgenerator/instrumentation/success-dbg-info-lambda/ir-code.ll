; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4, !dbg !0
@printf.str.1 = private unnamed_addr constant [19 x i8] c"Doubled twice: %d\0A\00", align 4, !dbg !5
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Is even: %d\0A\00", align 4, !dbg !9

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %0, i32 noundef %1) #0 !dbg !20 {
  %fct = alloca { ptr, ptr, i64 }, align 8
  %value = alloca i32, align 4
    #dbg_declare(ptr %fct, !33, !DIExpression(), !34)
  store { ptr, ptr, i64 } %0, ptr %fct, align 8, !dbg !34
    #dbg_declare(ptr %value, !35, !DIExpression(), !36)
  store i32 %1, ptr %value, align 4, !dbg !36
  %3 = load i32, ptr %value, align 4, !dbg !37
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !37
  %captures = load ptr, ptr %4, align 8, !dbg !37
  %fct1 = load ptr, ptr %fct, align 8, !dbg !37
  %5 = call i32 %fct1(i32 %3, ptr %captures), !dbg !37
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fct, i32 0, i32 1, !dbg !37
  %captures2 = load ptr, ptr %6, align 8, !dbg !37
  %fct3 = load ptr, ptr %fct, align 8, !dbg !37
  %7 = call i32 %fct3(i32 %5, ptr %captures2), !dbg !37
  ret i32 %7, !dbg !38
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 !dbg !39 {
  %counter = alloca i32, align 4
  %offset = alloca i64, align 8
  %captures = alloca { ptr, i64 }, align 8
  %fat.ptr = alloca { ptr, ptr, i64 }, align 8
  %increment = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr2 = alloca { ptr, ptr, i64 }, align 8
  %doubleFct = alloca { ptr, ptr, i64 }, align 8
  %fat.ptr3 = alloca { ptr, ptr, i64 }, align 8
  %isEven = alloca { ptr, ptr, i64 }, align 8
    #dbg_declare(ptr %counter, !52, !DIExpression(), !53)
  store i32 10, ptr %counter, align 4, !dbg !53
    #dbg_declare(ptr %offset, !54, !DIExpression(), !55)
  store i64 5, ptr %offset, align 8, !dbg !55
  store ptr %counter, ptr %captures, align 8, !dbg !56
  %1 = load i64, ptr %offset, align 8, !dbg !56
  %2 = getelementptr inbounds nuw { ptr, i64 }, ptr %captures, i32 0, i32 1, !dbg !56
  store i64 %1, ptr %2, align 8, !dbg !56
  store ptr @_Z15lambda.L10C21.0v, ptr %fat.ptr, align 8, !dbg !56
  %3 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 1, !dbg !56
  store ptr %captures, ptr %3, align 8, !dbg !56
  %4 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr, i32 0, i32 2, !dbg !56
  store i64 16, ptr %4, align 8, !dbg !56
    #dbg_declare(ptr %increment, !57, !DIExpression(), !56)
  %5 = load { ptr, ptr, i64 }, ptr %fat.ptr, align 8, !dbg !56
  store { ptr, ptr, i64 } %5, ptr %increment, align 8, !dbg !56
  %6 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %increment, i32 0, i32 1, !dbg !58
  %captures1 = load ptr, ptr %6, align 8, !dbg !58
  %fct = load ptr, ptr %increment, align 8, !dbg !58
  call void %fct(ptr %captures1), !dbg !58
  %7 = load i32, ptr %counter, align 4, !dbg !59
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %7), !dbg !59
  store ptr @_Z15lambda.L18C29.0i, ptr %fat.ptr2, align 8, !dbg !60
  %9 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 1, !dbg !60
  store ptr null, ptr %9, align 8, !dbg !60
  %10 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr2, i32 0, i32 2, !dbg !60
  store i64 0, ptr %10, align 8, !dbg !60
    #dbg_declare(ptr %doubleFct, !61, !DIExpression(), !60)
  %11 = load { ptr, ptr, i64 }, ptr %fat.ptr2, align 8, !dbg !60
  store { ptr, ptr, i64 } %11, ptr %doubleFct, align 8, !dbg !60
  %12 = load { ptr, ptr, i64 }, ptr %doubleFct, align 8, !dbg !62
  %13 = load i32, ptr %counter, align 4, !dbg !63
  %14 = call noundef i32 @_Z10applyTwicePFiiEi({ ptr, ptr, i64 } noundef %12, i32 noundef %13), !dbg !63
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %14), !dbg !63
  store ptr @_Z15lambda.L24C27.0i, ptr %fat.ptr3, align 8, !dbg !64
  %16 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 1, !dbg !64
  store ptr null, ptr %16, align 8, !dbg !64
  %17 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %fat.ptr3, i32 0, i32 2, !dbg !64
  store i64 0, ptr %17, align 8, !dbg !64
    #dbg_declare(ptr %isEven, !65, !DIExpression(), !64)
  %18 = load { ptr, ptr, i64 }, ptr %fat.ptr3, align 8, !dbg !64
  store { ptr, ptr, i64 } %18, ptr %isEven, align 8, !dbg !64
  %19 = load i32, ptr %counter, align 4, !dbg !66
  %20 = getelementptr inbounds nuw { ptr, ptr, i64 }, ptr %isEven, i32 0, i32 1, !dbg !66
  %captures4 = load ptr, ptr %20, align 8, !dbg !66
  %fct5 = load ptr, ptr %isEven, align 8, !dbg !66
  %21 = call i1 %fct5(i32 %19, ptr %captures4), !dbg !66
  %22 = zext i1 %21 to i32, !dbg !66
  %23 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %22), !dbg !66
  ret i32 0, !dbg !67
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z15lambda.L10C21.0v(ptr noundef nonnull dereferenceable(8) %0) #0 !dbg !68 {
  %captures = alloca ptr, align 8
  store ptr %0, ptr %captures, align 8, !dbg !72
  %2 = load ptr, ptr %captures, align 8, !dbg !72
    #dbg_declare(ptr %2, !73, !DIExpression(), !72)
  %offset = getelementptr inbounds nuw { ptr, i64 }, ptr %2, i32 0, i32 1, !dbg !72
    #dbg_declare(ptr %offset, !74, !DIExpression(), !72)
  %3 = load ptr, ptr %2, align 8, !dbg !75
  %4 = load i32, ptr %3, align 4, !dbg !75
  %5 = add nsw i32 %4, 1, !dbg !75
  store i32 %5, ptr %3, align 4, !dbg !75
  %6 = load i64, ptr %offset, align 8, !dbg !76
  %7 = trunc i64 %6 to i32, !dbg !76
  %8 = load ptr, ptr %2, align 8, !dbg !76
  %9 = load i32, ptr %8, align 4, !dbg !76
  %10 = add nsw i32 %9, %7, !dbg !76
  store i32 %10, ptr %8, align 4, !dbg !76
  ret void, !dbg !77
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @_Z15lambda.L18C29.0i(i32 %0, ptr %1) #0 !dbg !78 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !82, !DIExpression(), !83)
  store i32 %0, ptr %x, align 4, !dbg !83
  store ptr %1, ptr %captures, align 8, !dbg !83
  %3 = load i32, ptr %x, align 4, !dbg !84
  %4 = mul nsw i32 %3, 2, !dbg !84
  ret i32 %4, !dbg !85
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i1 @_Z15lambda.L24C27.0i(i32 %0, ptr %1) #0 !dbg !86 {
  %x = alloca i32, align 4
  %captures = alloca ptr, align 8
    #dbg_declare(ptr %x, !91, !DIExpression(), !92)
  store i32 %0, ptr %x, align 4, !dbg !92
  store ptr %1, ptr %captures, align 8, !dbg !92
  %3 = load i32, ptr %x, align 4, !dbg !93
  %4 = srem i32 %3, 2, !dbg !93
  %5 = icmp eq i32 %4, 0, !dbg !94
  ret i1 %5, !dbg !94
}

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!13, !14, !15, !16, !17, !18}
!llvm.ident = !{!19}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !7, line: 15, type: !12, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [host] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/test/./test-files/irgenerator/instrumentation/success-dbg-info-lambda/source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!4 = !{!0, !5, !9}
!5 = !DIGlobalVariableExpression(var: !6, expr: !DIExpression())
!6 = distinct !DIGlobalVariable(name: "printf.str.1", linkageName: "printf.str.1", scope: !2, file: !7, line: 21, type: !8, isLocal: true, isDefinition: true)
!7 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-lambda")
!8 = !DIStringType(name: "printf.str.1", size: 152)
!9 = !DIGlobalVariableExpression(var: !10, expr: !DIExpression())
!10 = distinct !DIGlobalVariable(name: "printf.str.2", linkageName: "printf.str.2", scope: !2, file: !7, line: 25, type: !11, isLocal: true, isDefinition: true)
!11 = !DIStringType(name: "printf.str.2", size: 104)
!12 = !DIStringType(name: "printf.str.0", size: 104)
!13 = !{i32 8, !"PIC Level", i32 2}
!14 = !{i32 7, !"PIE Level", i32 2}
!15 = !{i32 7, !"uwtable", i32 2}
!16 = !{i32 7, !"frame-pointer", i32 0}
!17 = !{i32 7, !"Dwarf Version", i32 5}
!18 = !{i32 2, !"Debug Info Version", i32 3}
!19 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
!20 = distinct !DISubprogram(name: "applyTwice", linkageName: "_Z10applyTwicePFiiEi", scope: !7, file: !7, line: 3, type: !21, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !32)
!21 = !DISubroutineType(types: !22)
!22 = !{!23, !24, !23}
!23 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!24 = !DICompositeType(tag: DW_TAG_structure_type, name: "_lambda", scope: !7, file: !7, size: 192, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !25, identifier: "_lambda")
!25 = !{!26, !29, !30}
!26 = !DIDerivedType(tag: DW_TAG_member, name: "fct", scope: !24, file: !7, baseType: !27, size: 64, align: 64)
!27 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64, align: 64)
!28 = !DIBasicType(name: "void", encoding: DW_ATE_unsigned)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "captures", scope: !24, file: !7, baseType: !27, size: 64, align: 64, offset: 64)
!30 = !DIDerivedType(tag: DW_TAG_member, name: "captureSize", scope: !24, file: !7, baseType: !31, size: 64, align: 64, offset: 128)
!31 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!32 = !{}
!33 = !DILocalVariable(name: "fct", arg: 1, scope: !20, file: !7, line: 3, type: !24)
!34 = !DILocation(line: 3, column: 19, scope: !20)
!35 = !DILocalVariable(name: "value", arg: 2, scope: !20, file: !7, line: 3, type: !23)
!36 = !DILocation(line: 3, column: 36, scope: !20)
!37 = !DILocation(line: 4, column: 20, scope: !20)
!38 = !DILocation(line: 5, column: 1, scope: !20)
!39 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !7, file: !7, line: 7, type: !40, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !42)
!40 = !DISubroutineType(types: !41)
!41 = !{!23}
!42 = !{!43, !50, !51}
!43 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !7, line: 10, size: 128, align: 64, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !44)
!44 = !{!45, !47}
!45 = !DIDerivedType(tag: DW_TAG_member, name: "counter", scope: !43, file: !7, line: 10, baseType: !46, size: 64)
!46 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !23, size: 64)
!47 = !DIDerivedType(tag: DW_TAG_member, name: "offset", scope: !43, file: !7, line: 10, baseType: !48, offset: 64)
!48 = !DIDerivedType(tag: DW_TAG_const_type, baseType: !49)
!49 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!50 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !7, line: 18, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !32)
!51 = !DICompositeType(tag: DW_TAG_class_type, scope: !39, file: !7, line: 24, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !32)
!52 = !DILocalVariable(name: "counter", scope: !39, file: !7, line: 8, type: !23)
!53 = !DILocation(line: 8, column: 19, scope: !39)
!54 = !DILocalVariable(name: "offset", scope: !39, file: !7, line: 9, type: !48)
!55 = !DILocation(line: 9, column: 25, scope: !39)
!56 = !DILocation(line: 10, column: 21, scope: !39)
!57 = !DILocalVariable(name: "increment", scope: !39, file: !7, line: 10, type: !24)
!58 = !DILocation(line: 14, column: 5, scope: !39)
!59 = !DILocation(line: 15, column: 29, scope: !39)
!60 = !DILocation(line: 18, column: 29, scope: !39)
!61 = !DILocalVariable(name: "doubleFct", scope: !39, file: !7, line: 18, type: !24)
!62 = !DILocation(line: 21, column: 46, scope: !39)
!63 = !DILocation(line: 21, column: 57, scope: !39)
!64 = !DILocation(line: 24, column: 27, scope: !39)
!65 = !DILocalVariable(name: "isEven", scope: !39, file: !7, line: 24, type: !24)
!66 = !DILocation(line: 25, column: 36, scope: !39)
!67 = !DILocation(line: 26, column: 1, scope: !39)
!68 = distinct !DISubprogram(name: "lambda.L10C21", linkageName: "_Z15lambda.L10C21.0v", scope: !43, file: !7, line: 10, type: !69, scopeLine: 10, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !32)
!69 = !DISubroutineType(types: !70)
!70 = !{!28, !71}
!71 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !43, size: 64)
!72 = !DILocation(line: 10, column: 21, scope: !68)
!73 = !DILocalVariable(name: "counter", scope: !68, file: !7, line: 8, type: !23)
!74 = !DILocalVariable(name: "offset", scope: !68, file: !7, line: 9, type: !48)
!75 = !DILocation(line: 11, column: 9, scope: !68)
!76 = !DILocation(line: 12, column: 9, scope: !68)
!77 = !DILocation(line: 13, column: 5, scope: !68)
!78 = distinct !DISubprogram(name: "lambda.L18C29", linkageName: "_Z15lambda.L18C29.0i", scope: !50, file: !7, line: 18, type: !79, scopeLine: 18, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !32)
!79 = !DISubroutineType(types: !80)
!80 = !{!23, !81, !23}
!81 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !50, size: 64)
!82 = !DILocalVariable(name: "x", arg: 1, scope: !78, file: !7, line: 18, type: !23)
!83 = !DILocation(line: 18, column: 29, scope: !78)
!84 = !DILocation(line: 19, column: 20, scope: !78)
!85 = !DILocation(line: 20, column: 5, scope: !78)
!86 = distinct !DISubprogram(name: "lambda.L24C27", linkageName: "_Z15lambda.L24C27.0i", scope: !51, file: !7, line: 24, type: !87, scopeLine: 24, flags: DIFlagPrototyped, spFlags: DISPFlagLocalToUnit | DISPFlagDefinition, unit: !2, retainedNodes: !32)
!87 = !DISubroutineType(types: !88)
!88 = !{!89, !90, !23}
!89 = !DIBasicType(name: "bool", size: 8, encoding: DW_ATE_boolean)
!90 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !51, size: 64)
!91 = !DILocalVariable(name: "x", arg: 1, scope: !86, file: !7, line: 24, type: !23)
!92 = !DILocation(line: 24, column: 27, scope: !86)
!93 = !DILocation(line: 24, column: 42, scope: !86)
!94 = !DILocation(line: 24, column: 47, scope: !86)
